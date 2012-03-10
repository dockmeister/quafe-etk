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

#include "keyinfo.h"
#include "exception.h"

#include <assert.h>
#include <boost/lexical_cast.hpp>
#include <glibmm/miscutils.h>

namespace EAPI {

ApiConfig config = {
		"apikey.xml",
		"/account/APIKeyInfo.xml.aspx",
		CACHE_STYLE_SHORT
};

KeyInfo * KeyInfo::create(int keyID, const Glib::ustring &vCode) {
	KeyInfo *key = 0;
	if(KeyInfo::find(keyID, key)) {
		throw Exception("KeyInfo already exists");
	}

	std::stringstream ss;
	ss << keyID << "_" << config.filename;
	config.filename = ss.str();

	return KeyInfo::manage(new KeyInfo(keyID, vCode));
}

KeyInfo::KeyInfo(int keyID, const Glib::ustring &vCode) :
	BasicAPI(keyID, config) {

	m_value_map["keyID"] = keyID;
	m_value_map["vCode"] = vCode;

	parse_cache();
}

KeyInfo::~KeyInfo() {
	DLOG("destroying keyinfo" << id);
}

bool KeyInfo::find_character(int characterID) const {
	const_iterator it = std::find_if(m_charlist.begin(), m_charlist.end(), boost::bind(&Character::id, _1) == characterID);
	return (it != m_charlist.end() && (*it).active);
}

const KeyInfo::Character & KeyInfo::get_character(int characterID) const {
	assert(m_charlist.size() > 0);
	const_iterator it = std::find_if(m_charlist.begin(), m_charlist.end(), boost::bind(&Character::id, _1) == characterID);
	assert(it != m_charlist.end());
	return (*it);
}

const KeyInfo::List & KeyInfo::get_character_list() const {
	return m_charlist;
}

bool KeyInfo::check_cakAccess(int access) {
	return true;
}

//	<result>
//		<key accessMask="268435455" type="Account" expires="">
//			<rowset name="characters" key="characterID" columns="characterID,characterName,corporationID,corporationName">
//				<row characterID="123456789" characterName="Character Name" corporationID="1000014" corporationName="Perkone" />
//				<row characterID="987654321" characterName="Name Character" corporationID="1000015" corporationName="KalaiDingsDa" />
//			</rowset>
//		</key>
//	</result>
bool KeyInfo::parse_result(const pugi::xml_node &result) {
	assert(result); // once this method is called it should be a valid api v2 document

	m_value_map["accessMask"] = result.child("key").attribute("accessMask").as_int();

	m_charlist.clear();
	pugi::xml_node char_rowset = result.child("key").child("rowset");
	std::for_each(char_rowset.begin(), char_rowset.end(), boost::bind(&KeyInfo::parse_character_row, this, _1));

	m_value_map["characters"] = &m_charlist;
	return true;
}

void KeyInfo::parse_character_row(const pugi::xml_node &char_row) {
	//<row characterID="123456789" characterName="Character Name" corporationID="1000014" corporationName="Perkone" />
	Character c;
	c.active = true;
	c.id = char_row.attribute("characterID").as_int();
	c.name = char_row.attribute("characterName").value();
	DLOG("char found: " << c.name);
	m_charlist.push_back(c);
}
}
