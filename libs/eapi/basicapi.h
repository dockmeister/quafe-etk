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

#ifndef BASICAPI_H_
#define BASICAPI_H_

#include "eapi-config.h"
#include "exception.h"

#include <map>
#include <boost/any.hpp>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <pugixml/pugixml.hpp>
#include <glibmm/datetime.h>
#include <glibmm/timezone.h>
#include <glibmm/thread.h>

namespace EAPI {


typedef struct {
	Glib::ustring filename;
	const Glib::ustring uri;
	const CacheStyle cache;
} ApiConfig;

typedef std::map<Glib::ustring, boost::any> ValueMap;

class BasicAPI : public boost::noncopyable {
	friend class Request;
public:
	const int id; //!<
	const Glib::ustring uri; //!< url suffix without domain 'api.eveonline.com'. e.g. /account/APIKeyInfo.xml.aspx
	const CacheStyle cache; //!< eve online caching policy
	Glib::ustring m_error; //!<


public:
	//! destructor
	virtual ~BasicAPI();

	/*! \brief Check this sheet for specfic flags
	 * \code
	 * check_status(API_STATUS_VALID | API_STATUS_OUTDATED);
	 * \endcode
	 *  returns != 0 if the sheet is valid AND outdated
	 * \see StatusFlags
	 * \param status_flags
	 * \return
	 */
	virtual int check_status(int status_flags);

	virtual void update(update_callback_t callback_);

	template <typename T>
	const T operator[](const Glib::ustring &) const;

protected:
	BasicAPI(const int id_, const ApiConfig &cfg);

	Glib::ustring m_filename; //!< local filename to save the xml file (workingdir + apikey_dir + file)
	int m_status;
	Glib::ustring m_errno; //!<


	pugi::xml_document m_document;
	ValueMap m_value_map;


protected:
	/*! \brief Parses the actual result row in a valid APIv2 document
	 *
	 * \param result_row a reference to the '<result>' node in a valid APIv2 Document
	 * \return true if the parse was successfull
	 */
	virtual bool parse_result(const pugi::xml_node &result_row) = 0;

	virtual void parse_cache();


	virtual bool parse_stringstream();
	virtual bool parse_xml_document();
private:
	Glib::Mutex lock_;
	Glib::Mutex lock_stream;
	std::stringstream stream_;

public:
	//! \brief returns the xml document as ustring
	virtual const Glib::ustring get_document();

	//! \brief Converts a ustring (format: 'YYYY-MM-DD HH:MM:SS') into DateTime
	Glib::DateTime ustring_to_datetime(const Glib::ustring &str) const;

	//! \brief used by the actual request - generates a string containing the post variables
	virtual const Glib::ustring get_postfields();
};

template <class T>
const T BasicAPI::operator [](const Glib::ustring &w) const {
	// TODO operator [] not thread safe
	ValueMap::const_iterator it = m_value_map.find(w);
	if(it == m_value_map.end()) {
		throw Exception("Value '" + w + "' not found. Check status first!");
	}
	try {
		return boost::any_cast<T>(it->second);
	} catch(...) {
		throw Exception("Bad any cast: '" +w+ "'");
	}
}
}

#endif /* BASICAPI_H_ */
