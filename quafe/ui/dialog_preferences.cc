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

#include "dialog_preferences.h"
#include "../utility.h"

#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/notebook.h>
#include <gtkmm/stock.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>

namespace Quafe {

PreferenceDialog::PreferenceDialog(const ustring &title, Gtk::Window &parent, bool modal) :
	Gtk::Dialog(title, parent, modal) {
	set_position(Gtk::WIN_POS_CENTER_ON_PARENT);

	add_button(Gtk::Stock::APPLY, Gtk::RESPONSE_APPLY);
	add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	Gtk::Box *vbox = get_vbox();

	Gtk::Notebook *nb = Gtk::manage(new Gtk::Notebook);
	nb->append_page(*create_general_tab(), "General");
	nb->append_page(*create_account_tab(), "Accounts");

	vbox->pack_start(*nb, Gtk::PACK_EXPAND_WIDGET);

	show_all_children(true);
}

PreferenceDialog::~PreferenceDialog() {
	// Gtk manages everything
}

Gtk::Widget * PreferenceDialog::create_general_tab() {
	return Gtk::manage(new Gtk::Button);
}

Gtk::Widget * PreferenceDialog::create_account_tab() {
	Gtk::Label *l1 = Gtk::manage(new Gtk::Label("Auth ID"));
	l1->set_hexpand(false);
	Gtk::Label *l2 = Gtk::manage(new Gtk::Label("Auth Key"));
	l2->set_hexpand(false);
	m_entry_auth_id = Gtk::manage(new Gtk::Entry);
	m_entry_auth_id->set_hexpand(false);
	m_entry_auth_key = Gtk::manage(new Gtk::Entry);
	m_entry_auth_key->set_hexpand(false);

	Gtk::Button *api_add = Gtk::manage(new Gtk::Button);
	api_add->set_image(*(Gtk::manage(new Gtk::Image(Gtk::Stock::ADD, Gtk::ICON_SIZE_SMALL_TOOLBAR))));
	api_add->signal_clicked().connect(sigc::bind<API_CHANGE>(sigc::mem_fun(this, &PreferenceDialog::add_api_button_clicked), API_ADD));

	Gtk::Button *api_del = Gtk::manage(new Gtk::Button);
	api_del->set_image(*(Gtk::manage(new Gtk::Image(Gtk::Stock::DELETE, Gtk::ICON_SIZE_SMALL_TOOLBAR))));
	api_del->signal_clicked().connect(sigc::bind<API_CHANGE>(sigc::mem_fun(this, &PreferenceDialog::add_api_button_clicked), API_DELETE));

	Gtk::ButtonBox *bbox = Gtk::manage(new Gtk::HButtonBox);
	bbox->set_hexpand(true);
	bbox->set_layout(Gtk::BUTTONBOX_CENTER);
	bbox->pack_start(*api_add);
	bbox->pack_start(*api_del);

	//Create the Tree model:
	m_refTreeModel = Gtk::TreeStore::create(m_columns);
	m_TreeView.set_model(m_refTreeModel);
	m_TreeView.set_hexpand(true);
	m_TreeView.set_vexpand(true);
	m_TreeView.append_column_editable("", m_columns.row_active);
	m_TreeView.append_column("Name", m_columns.name);
	m_TreeView.append_column("Auth ID", m_columns.auth_id);
	m_TreeView.append_column("Auth Key", m_columns.auth_key);

	Gtk::Grid *grid = Gtk::manage(new Gtk::Grid);
	grid->set_hexpand(true);
	grid->set_column_spacing(10);
	grid->set_row_spacing(10);
	grid->attach(*l1, 0, 0, 1, 1);
	grid->attach(*l2, 0, 1, 1, 1);
	grid->attach(*m_entry_auth_id, 1, 0, 1, 1);
	grid->attach(*m_entry_auth_key, 1, 1, 2, 1);
	grid->attach(*bbox, 2, 0, 1, 1);
	grid->attach(m_TreeView, 0, 2, 3, 1);

	return grid;
}
void PreferenceDialog::on_response(int response) {
	hide();
}

void PreferenceDialog::insert_api_row(Gtk::TreeModel::Row &row, gboolean active, ustring auth_id, ustring auth_key, ustring name) {
	row[m_columns.row_active] = active;
	row[m_columns.auth_id] = auth_id;
	row[m_columns.auth_key] = auth_key;
	row[m_columns.name] = name;
	m_TreeView.expand_all();
}

void PreferenceDialog::add_api_button_clicked(API_CHANGE e) {
	action_add_api_key(m_entry_auth_id->get_text(), m_entry_auth_key->get_text(), e);
}

} /* namespace Quafe */
