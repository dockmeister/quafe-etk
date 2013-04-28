/**
 *
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

#include "skilltraining.h"
#include "skilltraining-config.h"

#include "characternotebook.h"
#include "characterpage.h"

#include <boost/bind.hpp>
#include <eapi/sheets/charactersheet.h>

namespace Quafe {

PluginParams params = {
		"skilltraining",
		"Skill Training",
		"Training planer and more",
		"skilltraining.png",
		EAPI::CAK_CHARACTERSHEET | EAPI::CAK_SKILLQUEUE
	};

extern "C" const PluginParams & get_params() {
	return params;
}

extern "C" SkillTraining* create() {
    return new SkillTraining(params);
}

extern "C" void destroy(SkillTraining* p) {
    delete p;
}

SkillTraining::SkillTraining(const PluginParams &params)
	: PluginInterface(params), m_notebook(0) {

	init_glade("quafe-character.glade");
	m_refGlade->get_widget_derived("CharacterNotebook", m_notebook);

	m_sig_connections += EAPI::CharacterSheet::signal_sheet_added(boost::bind(&SkillTraining::on_charactersheet_added, this, _1));
	m_sig_connections += EAPI::CharacterSheet::signal_sheet_removed(boost::bind(&SkillTraining::on_charactersheet_removed, this, _1));
	//m_sig_connections += EAPI::CharacterSheet::signal_sheet_updated(boost::bind(&SkillTraining::on_charactersheet_updated, this, _1));
}

SkillTraining::~SkillTraining() {
}

bool SkillTraining::close() {
	m_sig_connections.block_all();
	return true;
}

Gtk::Widget& SkillTraining::show() {
	m_sig_connections.unblock_all();
	return *m_notebook;
}

void SkillTraining::hide() {

}

void SkillTraining::on_charactersheet_added(EAPI::CharacterSheet *sheet) {
	gint cid = sheet->value<gint>("id");
	Glib::ustring cname = sheet->value<Glib::ustring>("Character");
	CharacterPage *page = m_notebook->add_character_page(cid, cname);


	update_character_page(sheet, page);
}

void SkillTraining::on_charactersheet_removed(EAPI::CharacterSheet *sheet) {

}

void SkillTraining::update_character_page(EAPI::CharacterSheet *sheet, CharacterPage *page) {
	page->set_name(sheet->value<Glib::ustring>("CharacterName"));

}

}
