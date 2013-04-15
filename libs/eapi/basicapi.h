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

#include <eapi/eapi-config.h>
#include <eapi/types.h>
#include <eapi/exception.h>

#include <map>
#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <pugixml/pugixml.hpp>
#include <glibmm/datetime.h>
#include <glibmm/timezone.h>
#include <glibmm/thread.h>

namespace EAPI {

class BasicAPI: public boost::noncopyable {
	friend class Request;
	friend class Main;
public:
	typedef struct {
		Glib::ustring filename;
		const Glib::ustring uri;
		const CacheStyle cache;
	} ApiConfig;
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
	virtual ~BasicAPI();

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

	template<typename T>
	const T value(const Glib::ustring) const;

protected:
	/*!\brief
	 *
	 * \param id_ a unique id (e.g. characterid)
	 * \param ApiConfig
	 */
	BasicAPI(const int id_, const ApiConfig &cfg);

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

	virtual void finish() = 0;

	Glib::ustring m_filename; //!< local filename to save the xml file (workingdir + apikey_dir + file)
	int m_status;
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

	//! \brief used by the actual request - generates a string containing the post variables
	virtual const Glib::ustring get_postfields();
};

template<class T>
const T BasicAPI::value(const Glib::ustring w) const {
	// TODO operator [] not thread safe
	ValueMap::const_iterator it = m_value_map.find(w);
	if (it == m_value_map.end()) {
		throw BadValueException<T>("Value '" + w + "' not found. Check status first!");
	}
	try {
		return boost::any_cast<T>(it->second);
	} catch (...) {
		throw BadValueCastException<T>(w, T());
	}
}

}

#endif /* BASICAPI_H_ */
