/**
 *  Quafe - Eve tools for linux.
 *  Copyright (C) 2012 Christoph Nikic
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <eapi/apiinterface.h>
#include <eapi/eapi-logging.h>
#include <eapi/eapi.h>

#include <giomm/file.h>
#include <glibmm/fileutils.h>
#include <glibmm/miscutils.h>
#include <glibmm/datetime.h>

EAPI_DECLARE_STATIC_LOGGER("EAPI");

namespace EAPI {

APIInterface::APIInterface(const int id_, const Config &cfg) :
		id(id_), uri(cfg.uri), cache(cfg.cache){

	m_filename = Glib::build_filename(Main::instance()->get_working_dir(), cfg.filename);
	m_value_map["cachedUntil"] = Glib::DateTime::create_utc(0,0,0,0,0,0);
}


APIInterface::~APIInterface() {
}


int APIInterface::check_status(int status_flags) {
	if(!m_error.empty() || (m_status & API_STATUS_ERROR)) {
		assert((m_status & API_STATUS_VALID) == 0);
		assert((m_status & API_STATUS_ERROR) > 0);
		return (m_status & status_flags);
	}

	if(m_status & API_STATUS_VALID) {
		// once here we can assert cachedUntil is set
		Glib::DateTime cache = get_datetime("cachedUntil");
		Glib::TimeSpan diff  = cache.difference(Glib::DateTime::create_now_utc());
		if(diff < 0) {
			m_status |= API_STATUS_OUTDATED;
		} else {
			m_status |= API_STATUS_CACHED;
		}
	}

	return (m_status & status_flags);
}


void APIInterface::update() {
	if(!lock_.trylock()) {
		throw Exception("A update is already running!");
	}

	if(check_status(API_STATUS_CACHED) > 0) {
		throw NotOutdatedException();
	}

	// clearing any previous errors any run the request
	m_error.clear();
	m_errno.clear();

	Main::instance()->request(this);
}

void APIInterface::finish_update() {

}

void APIInterface::lock() {
	lock_.lock();
}

void APIInterface::unlock() {
	lock_.unlock();
}

void APIInterface::set_error(const Glib::ustring msg, APIError e) {
	m_error = msg;
}

/********************************************************************************************
 * Parse methods
 */

bool APIInterface::parse_cache() {
	if(!Glib::file_test(m_filename, Glib::FILE_TEST_EXISTS))
		return false;

	// Locks this sheet until this method is finished.
	Glib::Mutex::Lock lock(lock_);

	pugi::xml_parse_result result = m_document.load_file(m_filename.c_str());
	if (!result) {
		m_error = result.description();
		return false;
	}

	LOG_DEBUG("Document was found in cache");

	return parse_xml_document();
}


bool APIInterface::parse_stringstream() {
	pugi::xml_parse_result result = m_document.load(stream_);
	if (!result) {
		m_error = result.description();
		return false;
	}

	if(!parse_xml_document()) {
		return false;
	}

	// document was successfully retrieved and parsed -> cache the file
	Glib::ustring str_directory = Glib::path_get_dirname(m_filename);

	if(!Glib::file_test(str_directory, Glib::FILE_TEST_IS_DIR)) {
		Glib::RefPtr<Gio::File> dir = Gio::File::create_for_path(str_directory);
		if(!dir->make_directory_with_parents()) {
			m_error = "failed to write to cache";
			return false;
		}
	}

	m_document.save_file(m_filename.c_str());

	return true;
}


bool APIInterface::parse_xml_document() {
	int vremove = API_STATUS_OUTDATED & API_STATUS_CACHED & API_STATUS_ERROR;
	//[ Check if the document is a valid eveapi document
	pugi::xml_node root = m_document.child("eveapi");
	if (!root || root.attribute("version").as_int() != 2) {
		m_error = "The result is not a valid Eve API v2 document";
		m_status = (m_status & vremove) | API_STATUS_ERROR; // remove VALID-flag add ERROR-flag
		return false;
	}

	pugi::xml_node error = root.child("error");
	if (error) {
		m_errno = error.attribute("code").value();
		m_error = error.child_value();
		m_status = (m_status & vremove) | API_STATUS_ERROR; // remove VALID-flag add ERROR-flag
		return false;
	}
	//]

	//[ m_document is valid - parse the result
	m_value_map["age"] = ustring_to_datetime(Glib::ustring(root.child("currentTime").child_value()));
	m_value_map["cachedUntil"] = ustring_to_datetime(Glib::ustring(root.child("cachedUntil").child_value()));

	// let the actual api implementation do the rest
	if(parse_result(root.child("result"))) {
		m_status = API_STATUS_VALID; // remove ERROR-flag add VALID-flag
		return true;
	}

	m_status = (m_status & vremove) | API_STATUS_ERROR; // remove VALID-flag add ERROR-flag
	return false;
	//]
}

// ------------------------------------------------------------------------------------------
// - Value access methods

Glib::ustring APIInterface::get_string(const Glib::ustring what) const {
	Glib::ustring ret_value = "";
	try {
		ret_value = get_value<Glib::ustring>(what);
	} catch(Exception &e) {
		LOG_WARN(e.what());
	}

	return ret_value;
}

int APIInterface::get_integer(const Glib::ustring what) const {
	int ret_value = 0;
	try {
		ret_value = get_value<int>(what);
	} catch(Exception &e) {
		LOG_WARN(e.what());
	}

	return ret_value;
}

Glib::DateTime APIInterface::get_datetime(const Glib::ustring what) const {
	Glib::DateTime ret_value;
	try {
		ret_value = get_value<Glib::DateTime>(what);
	} catch(Exception &e) {
		LOG_WARN(e.what());
	}

	return ret_value;
}
// ------------------------------------------------------------------------------------------
// -

const Glib::ustring APIInterface::get_document() {
	Glib::Mutex::Lock lock(lock_);
	std::stringstream doc;
	m_document.save(doc);

	return doc.str();
}


Glib::DateTime APIInterface::ustring_to_datetime(const Glib::ustring &str_t) const {
	// input format: YYYY-MM-DD HH:MM:SS
	Glib::DateTime date_t;
	try {
		int ye = boost::lexical_cast<int>(str_t.substr(0, 4));
		int mo = boost::lexical_cast<int>(str_t.substr(5, 2));
		int da = boost::lexical_cast<int>(str_t.substr(8, 2));
		int ho = boost::lexical_cast<int>(str_t.substr(11, 2));
		int mi = boost::lexical_cast<int>(str_t.substr(14, 2));
		int se = boost::lexical_cast<int>(str_t.substr(17, 2));
		date_t = Glib::DateTime::create_utc(ye, mo, da, ho, mi, se);
	} catch (boost::bad_lexical_cast &e) {
		date_t = Glib::DateTime::create_now_utc();
	}
	return date_t;
}


const Glib::ustring APIInterface::make_postfields() const{
	Glib::ustring post;

	ValueMap::const_iterator it = m_value_map.find("keyID");
	if(it != m_value_map.end()) {
		post += Glib::ustring::compose("keyID=%1", get_integer("keyID"));
	}

	it = m_value_map.find("vCode");
	if(it != m_value_map.end()) {
		post += Glib::ustring::compose("&vCode=%1", get_string("vCode"));
	}

	it = m_value_map.find("characterID");
	if(it != m_value_map.end()) {
		post += Glib::ustring::compose("&characterID=%1", get_integer("characterID"));
	}

	return post;
}

const Glib::ustring APIInterface::make_url() const {
	return Glib::ustring::compose("https://api.eveonline.com%1", uri);
}
}
