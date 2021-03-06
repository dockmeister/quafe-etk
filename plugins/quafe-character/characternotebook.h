/*
 * characternotebook.h
 *
 *  Created on: 17.02.2013
 *      Author: cn
 */

#ifndef CHARACTERNOTEBOOK_H_
#define CHARACTERNOTEBOOK_H_

#include "characterpage.h"

#include <plugins/pluginwidget.h>

#include <map>
#include <gtkmm/notebook.h>


namespace Quafe {

class CharacterNotebook : public PluginWidget, public Gtk::Notebook {
public:
	CharacterNotebook(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
	virtual ~CharacterNotebook();

	CharacterPage* add_character_page(gint id, const Glib::ustring &tab_label);
	bool find_character_page(gint id, CharacterPage *&page);

private:
	typedef std::map<gint, CharacterPage *> CharacterPageMap;
	std::map<gint, CharacterPage *> m_cpage_list;
};

} /* namespace Quafe */
#endif /* CHARACTERNOTEBOOK_H_ */
