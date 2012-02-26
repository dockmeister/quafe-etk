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

#include "window_preferences.h"

#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/notebook.h>
#include <gtkmm/stock.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>

namespace Quafe {

WindowPreferences::WindowPreferences() {
	set_title("Preferences");
	set_position(Gtk::WIN_POS_CENTER_ALWAYS);
	set_type_hint(Gdk::WINDOW_TYPE_HINT_DIALOG);
	set_skip_taskbar_hint(true);
	set_modal(true);

	Gtk::VBox *vbox = Gtk::manage(new Gtk::VBox);

	Gtk::Notebook *nb = Gtk::manage(new Gtk::Notebook);
	nb->append_page(*create_general_tab(), "General");
	nb->append_page(*create_account_tab(), "Accounts");

	Gtk::Button *b_ok = Gtk::manage(new Gtk::Button(Gtk::Stock::OK));
	b_ok->signal_clicked().connect(action_ok);

	Gtk::Button *b_ca = Gtk::manage(new Gtk::Button(Gtk::Stock::CANCEL));
	b_ca->signal_clicked().connect(action_cancel);

	Gtk::HButtonBox *b_box = Gtk::manage(new Gtk::HButtonBox);
	b_box->set_layout(Gtk::BUTTONBOX_END);
	b_box->pack_start(*b_ca, Gtk::PACK_SHRINK);
	b_box->pack_start(*b_ok, Gtk::PACK_SHRINK);

	vbox->pack_start(*nb, Gtk::PACK_EXPAND_WIDGET);
	vbox->pack_start(*b_box, Gtk::PACK_SHRINK);

	add(*vbox);
	show_all_children(true);
}

WindowPreferences::~WindowPreferences() {
	// Gtk manages everything
}

Gtk::Widget * WindowPreferences::create_general_tab() {
	return Gtk::manage(new Gtk::Button);
}

Gtk::Widget * WindowPreferences::create_account_tab() {
	Gtk::Grid *grid = Gtk::manage(new Gtk::Grid);
	Gtk::Label *l1 = Gtk::manage(new Gtk::Label("Auth ID"));
	Gtk::Label *l2 = Gtk::manage(new Gtk::Label("Auth Key"));
	Gtk::Entry *e1 = Gtk::manage(new Gtk::Entry);
	Gtk::Entry *e2 = Gtk::manage(new Gtk::Entry);
	Gtk::ButtonBox *bbox = Gtk::manage(new Gtk::ButtonBox);


	grid->attach(*l1, 0, 0, 1, 1);
	grid->attach(*l2, 0, 1, 1, 1);
	grid->attach(*e1, 1, 0, 1, 1);
	grid->attach(*e2, 1, 1, 2, 1);
	return grid;
}

} /* namespace Quafe */
