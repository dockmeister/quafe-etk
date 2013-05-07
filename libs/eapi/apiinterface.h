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

#ifndef APIINTERFACE_H_
#define APIINTERFACE_H_

#include <eapi/eapi-config.h>
#include <eapi/types.h>
#include <eapi/exception.h>

#include <map>
#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/function.hpp>
#include <boost/signals2.hpp>
#include <pugixml/pugixml.hpp>
#include <glibmm/datetime.h>
#include <glibmm/timezone.h>
#include <glibmm/thread.h>

namespace EAPI {

class EAPI_API APIInterface {
public:
	struct Config {
		Glib::ustring filename;
		const Glib::ustring uri;
		const CacheStyle cache;
	};

protected:
	//!< The type ValueMap stores any value parsed from xml
	typedef std::map<Glib::ustring, boost::any> ValueMap;

public:
	const int id; //!< stores a unique id, usually the character id (except for KeyInfo)
	const Glib::ustring uri; //!< url suffix without domain 'api.eveonline.com'. e.g. /account/APIKeyInfo.xml.aspx
	const CacheStyle cache; //!< eve online caching policy
	Glib::ustring m_error; //!<

public:
	//!< public destructor
	virtual ~APIInterface();

	//!< remove copy constructor
	APIInterface(const APIInterface &other) = delete;

	//!<
	const APIInterface& operator=( const APIInterface& ) = delete;

	/*!\brief Check this sheet for specfic flags
	 *
	 * \see StatusFlags
	 * \param flags a bitwise OR operation of StatusFlags
	 * \return a bitwise AND comparison
	 */
	virtual int check_status(int flags);

	/*!\brief
	 *
	 * \throws NotOutdatedException
	 */
	virtual void update();

	//!
	Glib::ustring get_string(const Glib::ustring w) const;

	//!
	int32_t get_integer(const Glib::ustring w) const;

	//!
	Glib::DateTime get_datetime(const Glib::ustring w) const;

	//!
	template <class V>
	V get_value(const Glib::ustring w) const;

protected:
	/*!\brief
	 *
	 * \param id_ a unique id (e.g. characterid)
	 * \param ApiConfig
	 */
	APIInterface(const int id_, const Config &cfg);

	/*!\brief Checks if a cached EAPI document is available. If so parse_xml_document will be called.
	 *
	 * \returns whether the parse was successful
	 */
	virtual bool parse_cache();

	/*!\brief Parses the actual result row in a valid APIv2 document, this method has to be
	 * implemented in the actual Sheet implementation
	 *
	 * \param result_row a reference to the '<result>' node in a valid APIv2 Document
	 * \return true if the parse was successful
	 */
	virtual bool parse_result(const pugi::xml_node &result_row) = 0;

	virtual bool parse_stringstream();
	virtual bool parse_xml_document();

	virtual void finish(const APIEvent) = 0;

	virtual void finish_update();
	virtual void lock();
	virtual void unlock();

	virtual void set_error(const Glib::ustring msg, APIError e = 0);

	Glib::ustring m_filename; //!< local filename to save the xml file (workingdir + apikey_dir + file)
	int m_status = 0;
	Glib::ustring m_errno; //!<

	pugi::xml_document m_document;
	ValueMap m_value_map;

private:
	Glib::Mutex lock_;
	std::stringstream stream_;

public:
	//! \brief returns the xml document as ustring
	virtual const Glib::ustring get_document();

	//! \brief Converts aGlib::ustring  (format: 'YYYY-MM-DD HH:MM:SS') into DateTime
	Glib::DateTime ustring_to_datetime(const Glib::ustring &str) const;

protected:
	//! \brief used by the actual request - generates a string containing the post variables
	virtual const Glib::ustring make_postfields() const;
	virtual const Glib::ustring make_url() const;
};

template <class V>
V APIInterface::get_value<V>(const Glib::ustring w) const {
	auto itr = m_value_map.find(w);
	if(itr == m_value_map.find(w)) {
		throw BadValueException("Invalid access to index: " + w);
	}

	try {
		return boost::any_cast<V>(itr->second);
	} catch (boost::bad_any_cast &e) {
		throw BadValueCastException("Failed to cast index %1 to string" + w);
	}
}
}

#endif /* APIINTERFACE_H_ */
