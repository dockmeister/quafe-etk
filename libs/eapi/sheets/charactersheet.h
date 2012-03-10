/*
 * charactersheet.h
 *
 *  Created on: 06.03.2012
 *      Author: cn
 */

#ifndef CHARACTERSHEET_H_
#define CHARACTERSHEET_H_

#include "../eapi-config.h"
#include "../basicapi.h"
#include "../sheetlist.h"

namespace EAPI {
class KeyInfo;

/*
 *
 */
class CharacterSheet: public SheetList<CharacterSheet>, public BasicAPI {
public:
	static CharacterSheet * create(int charcter_id);
public:
	CharacterSheet(int character_id, const Glib::ustring &name, const KeyInfo *key);
	virtual ~CharacterSheet();
protected:
	virtual bool parse_result(const pugi::xml_node &result) {
		return true;
	}
private:
	const Glib::ustring character_name;
};

} /* namespace EAPI */
#endif /* CHARACTERSHEET_H_ */
