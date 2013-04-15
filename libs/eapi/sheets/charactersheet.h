/*
 * charactersheet.h
 *
 *  Created on: 06.03.2012
 *      Author: cn
 */

#ifndef CHARACTERSHEET_H_
#define CHARACTERSHEET_H_

#include <eapi/eapi-config.h>
#include <eapi/basicapi.h>
#include <eapi/sheetlist.h>

namespace EAPI {

class KeyInfo;
/*!
 *
 */
class CharacterSheet: public SheetList<CharacterSheet>, public BasicAPI {
public:
	static CharacterSheet * create(int charcter_id);
public:
	virtual ~CharacterSheet();
	virtual void update(const CharacterSheet::sheet_slot_t &slot);
protected:
	CharacterSheet(int character_id, const Glib::ustring &name, const KeyInfo *key);
	virtual void finish();
	virtual bool parse_result(const pugi::xml_node &result);
private:
	const Glib::ustring character_name;
};

} /* namespace EAPI */
#endif /* CHARACTERSHEET_H_ */
