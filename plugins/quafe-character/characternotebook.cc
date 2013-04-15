/*
 * characternotebook.cc
 *
 *  Created on: 17.02.2013
 *      Author: cn
 */

#include "characternotebook.h"

namespace Quafe {
	namespace Plugin {

CharacterNotebook::CharacterNotebook() {
	append_page(*Gtk::manage(new Gtk::Label("test")), "A Page", false);
}

CharacterNotebook::~CharacterNotebook() {
	// TODO Auto-generated destructor stub
}

CharacterPage* CharacterNotebook::add_character_page(gint id, const Glib::ustring &tab_label) {
	CharacterPage *page = 0;

	if(find_character_page(id, page)) {
		return page;
	} else {
		page = new CharacterPage();
		m_cpage_list[id] = page;
	}

	append_page(*page, tab_label);

	return page;
}

bool CharacterNotebook::find_character_page(gint id, CharacterPage *&page) {
	CharacterPageMap::iterator it = m_cpage_list.find(id);

	if(it == m_cpage_list.end()) {
		return false;
	}

	page = it->second;
	return true;
}
	}
} /* namespace Quafe */
