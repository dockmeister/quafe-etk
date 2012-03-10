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

#ifndef KEYINFO_H_
#define KEYINFO_H_

#include "../eapi-config.h"
#include "../basicapi.h"
#include "../sheetlist.h"

namespace EAPI {



class KeyInfo : public SheetList<KeyInfo>, public BasicAPI {
public:
	//! \brief Represents a character available through this API Key.
	struct Character {
		bool active;
		int id;
		Glib::ustring name;
	};
	typedef std::list<Character> List;
	typedef List::const_iterator const_iterator;
	typedef List::iterator iterator;

public:
	static KeyInfo * create(int keyID, const Glib::ustring &vCode);

	virtual ~KeyInfo();

	/*! \brief Searches the character list for 'id'
	 *
	 * \param id
	 * \return true if the character id was found and is active
	 */
	bool find_character(int characterID) const;

	const Character & get_character(int characterID) const;

	const List & get_character_list() const;

	/*!\brief Check the API Access mask against a given Mask
	 *
	 * @param access
	 * @return true if this API Key supports
	 */
	bool check_cakAccess(int access);
protected:
	//! \brief Use create to add a API Key.
	//! \see create
	KeyInfo(int id, const Glib::ustring &vCode);

	/*! \copydoc BasicAPI::parse_result()
	 *
	 * @param result_row
	 * @return
	 */
	virtual bool parse_result(const pugi::xml_node &result_row);
	virtual void parse_character_row(const pugi::xml_node &char_row);
private:
	List m_charlist;
	bool active;
};


}

#endif /* KEYINFO_H_ */
