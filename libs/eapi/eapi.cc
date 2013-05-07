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
#include <eapi/eapi.h>
#include <eapi/eapi-logging.h>

#include <eapi/request.h>
#include <eapi/apisheet.h>
#include <eapi/exception.h>

#include <assert.h>
#include <boost/bind.hpp>
#include <glibmm/fileutils.h>

EAPI_DECLARE_STATIC_LOGGER("EAPI");

namespace EAPI {
Main * Main::init(const Glib::ustring &wdir) {
	try {
		if (!Glib::thread_supported()) {
			Glib::thread_init();
		}
	} catch (Glib::Error &e) {
		//LOG_FATAL("Failed to initialize %1", "Glib");
		//LOG_FATAL("__BASE_FILE__");
		//Glib::ustring::compose("test", e);
		throw Exception("Failed to initialize Glib thread system.");
	}

	if (!Glib::file_test(wdir, Glib::FILE_TEST_IS_DIR)) {
		// TODO check if dir is writable
		// TODO throw eapi::exception
	}

	curl_global_init(CURL_GLOBAL_ALL);

	Request::init_request_system();
	Main *eapi = Main::instance();
	eapi->set_working_dir(wdir);

	return eapi;
}

Main::Main() :
	m_thread_pool(NUM_MAX_THREADS), verbose_(true){

	Request::signal_debug().connect(boost::bind(&Main::curl_debug_recieved, this));
	Request::signal_finished().connect(boost::bind(&Main::request_finished, this));
}

Main::~Main() {
	LOG_INFO("Shutting down EAPI");
	m_thread_pool.shutdown();
	Request::destroy_request_system();
}

void Main::request(APIInterface *api) {
	LOG_DEBUG("Pushing new API Request to queue. (verbose: " << verbose_ << ")");
	Request *req = new Request(api, verbose_);

	if (!req->validate()) {
		throw Exception("something went seriously wrong");
	}

	m_thread_pool.push(boost::bind(&Request::run, req));
}

void Main::request_finished() {
	if(Request::num_finish_queue() <= 0)
		return;

	Request *req = Request::pop_finish_queue();
	EAPI_DELETE(req);
}

/********************************************************************************************
 * Methods to control debug output
 */

void Main::curl_debug_recieved() {
	if(Request::num_debug_queue() <= 0)
		return;

	Glib::ustring msg = Request::pop_debug_queue();
	try {
		while(size_t pos = msg.find('\n'))
			msg = msg.replace(pos, 1, " ");
	} catch(...) {}
	LOG_DEBUG(msg);
}


}
