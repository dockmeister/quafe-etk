/*
 * charactersheet.cc
 *
 *  Created on: 06.03.2012
 *      Author: cn
 */

#include <eapi/sheets/charactersheet.h>
#include <eapi/eapi-logging.h>

#include <eapi/sheets/keyinfo.h>
#include <eapi/exception.h>

#include <assert.h>
#include <glibmm/miscutils.h>

EAPI_DECLARE_STATIC_LOGGER("EAPI");

namespace EAPI {

CharacterSheet::ApiConfig charsheet_config = {
		"CharacterSheet.xml",
		"/char/CharacterSheet.xml.aspx",
		CACHE_STYLE_SHORT
};

CharacterSheet * CharacterSheet::create(int characterID) {
	CharacterSheet *cs = 0;
	if(CharacterSheet::find(characterID, cs)) {
		//LOG_DEBUG("Character (id:"<< characterID << ") already exists.");
		return cs;
	}

	KeyInfo *key = 0;
	if(!KeyInfo::find_if(boost::bind(&KeyInfo::find_character, _1, characterID), key)) {
		throw Exception("Character id not found or not active");
	}

	//LOG_TRACE("KeyInfo::find returned with unexpected behavior!");

	if(!key->check_cakAccess(CAK_CHARACTERSHEET)) {
		throw Exception("CharacterSheet is not supported.");
	}

	KeyInfo::Character c = key->get_character(characterID);

	charsheet_config.filename = Glib::build_filename(c.name, charsheet_config.filename);

	return CharacterSheet::manage(new CharacterSheet(characterID, c.name, key));
}

CharacterSheet::CharacterSheet(int characterID, const Glib::ustring &name, const KeyInfo *key) :
		BasicAPI(characterID, charsheet_config) {

	m_value_map["keyID"] = key->value<int>("keyID");
	m_value_map["vCode"] = key->value<Glib::ustring>("vCode");
	m_value_map["characterID"] = characterID;
	m_value_map["name"] = name;

	parse_cache();
}

CharacterSheet::~CharacterSheet() {
}


/********************************************************************************************
 * Parse & update methods
 *
 *	<result>
 *		<key accessMask="268435455" type="Account" expires="">
 *			<rowset name="characters" key="characterID" columns="characterID,characterName,corporationID,corporationName">
 *				<row characterID="123456789" characterName="Character Name" corporationID="1000014" corporationName="Perkone" />
 *				<row characterID="987654321" characterName="Name Character" corporationID="1000015" corporationName="KalaiDingsDa" />
 *			</rowset>
 *		</key>
 *	</result>
 */
void CharacterSheet::update(const CharacterSheet::sheet_slot_t &slot) {
	assert(!sig_connection.connected());
	sig_connection = signal_sheet_updated(slot);
	BasicAPI::update();
}

void CharacterSheet::finish() {
	m_signal_sheet_updated(this);
	if(sig_connection.connected()) {
		sig_connection.disconnect();
	}
}

bool CharacterSheet::parse_result(const pugi::xml_node &result) {
	return true;
}

} /* namespace EAPI */
