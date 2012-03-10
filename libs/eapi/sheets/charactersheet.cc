/*
 * charactersheet.cc
 *
 *  Created on: 06.03.2012
 *      Author: cn
 */

#include "charactersheet.h"
#include "keyinfo.h"
#include "exception.h"

#include <assert.h>

namespace EAPI {

ApiConfig charsheet_config = {
		"CharacterSheet.xml",
		"/char/CharacterSheet.xml.aspx",
		CACHE_STYLE_SHORT
};

CharacterSheet * CharacterSheet::create(int characterID) {
	CharacterSheet *cs = 0;
	if(CharacterSheet::find(characterID, cs)) {
		throw Exception("CharacterSheet already created.");
	}

	KeyInfo *key = 0;
	if(!KeyInfo::find_if(boost::bind(&KeyInfo::find_character, _1, characterID), key)) {
		throw Exception("Character id not found or not active");
	}

	if(!key)
		DLOG("ARGH 'find' doesnt work");

	if(!key->check_cakAccess(CAK_CHARACTERSHEET)) {
		throw Exception("CharacterSheet is not supported.");
	}

	DLOG("test");
	KeyInfo::Character c = key->get_character(characterID);


	charsheet_config.filename = Glib::build_filename(c.name, charsheet_config.filename);

	return CharacterSheet::manage(new CharacterSheet(characterID, c.name, key));
}

CharacterSheet::CharacterSheet(int characterID, const Glib::ustring &name, const KeyInfo *key) :
		BasicAPI(characterID, charsheet_config) {

	// TODO check why key<int>["keyID"] doenst work
	m_value_map["keyID"] = key->operator []<int>("keyID");
	m_value_map["vCode"] = key->operator []<Glib::ustring>("vCode");
	m_value_map["characterID"] = characterID;
	m_value_map["name"] = name;

	parse_cache();
}

CharacterSheet::~CharacterSheet() {
}

} /* namespace EAPI */
