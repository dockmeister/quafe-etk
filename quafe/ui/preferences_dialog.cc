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

#include <quafe-config.h>

#include "preferences_dialog.h"

#include <boost/bind.hpp>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/notebook.h>
#include <gtkmm/stock.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>

namespace Quafe {

PreferenceDialog::PreferenceDialog(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
	Gtk::Dialog(cobject) {

	refGlade->get_widget("DialogAPIKey", md_add_api_key);
	refGlade->get_widget("entry_keyid", m_entry_auth_id);
	refGlade->get_widget("entry_vcode", m_entry_auth_key);

	refGlade->get_widget("button_add_api", mb_api_add);
	mb_api_add->signal_clicked().connect(boost::bind(&PreferenceDialog::on_api_add_button_clicked, this));

	m_treestore_api = Glib::RefPtr<Gtk::TreeStore>::cast_static(refGlade->get_object("treestore_api"));
	refGlade->get_widget("api_view", m_api_view);

	m_refToggle	= Glib::RefPtr<Gtk::CellRendererToggle>::cast_static(refGlade->get_object("api_view_renderer_toggle"));
	m_refToggle->property_activatable() = true;
	m_refToggle->signal_toggled().connect(boost::bind(&PreferenceDialog::on_api_check_toggled, this, _1));


	//refGlade->get_widget("api_selection", m_api_selection);
	//refGlade->get_widget("api_view_column_toggle", m_api_view_column_toggle);
	//m_api_view_column_toggle->signal_clicked();
}

PreferenceDialog::~PreferenceDialog() {
	// Gtk manages everything
}

void PreferenceDialog::set_parent(Gtk::Window &parent) {
	set_transient_for(parent);
	//Gtk::Dialog::reparent(parent);
	//set_position(Gtk::WIN_POS_CENTER_ON_PARENT);
}

void PreferenceDialog::on_response(int response) {
	hide();
}

void PreferenceDialog::on_api_add_button_clicked() {
	int response = md_add_api_key->run();

	if(response == Gtk::RESPONSE_APPLY) {
		Glib::ustring keyID = m_entry_auth_id->get_text();
		Glib::ustring vCode = m_entry_auth_key->get_text();

		md_add_api_key->hide();

		if(!keyID.empty() && !vCode.empty())
			m_signal_api_add(keyID, vCode);
	} else {
		md_add_api_key->hide();
	}
}

void PreferenceDialog::on_api_check_toggled(const Glib::ustring &path) {
    Gtk::TreeModel::iterator iter = m_treestore_api->get_iter(path);

    if(!iter) {
    	return;
    }

    if((*iter)[m_columns.is_api] == true) {
    	// signal_toggle_api() ???
    	return;
    }

    if((*iter)[m_columns.is_api] == false && m_signal_toggle_character((*iter)[m_columns.keyID_t], (*iter)[m_columns.keyID], !(*iter)[m_columns.row_active])) {
   		(*iter)[m_columns.row_active] = !(*iter)[m_columns.row_active];
   		return;
    }
}

Gtk::TreeModel::Row PreferenceDialog::insert_api_row(Glib::ustring keyID, Glib::ustring vCode, gboolean active) {
	Gtk::TreeModel::Row api_row = *(m_treestore_api->append());
	api_row[m_columns.row_active] = active;
	api_row[m_columns.keyID] = keyID;
	api_row[m_columns.vCode] = vCode;
	api_row[m_columns.is_api] = true;
	api_row[m_columns.keyID_t] = keyID;

	m_api_view->expand_all();

	return api_row;
}

void PreferenceDialog::insert_character_row(Gtk::TreeModel::Row &api_row, Glib::ustring id, Glib::ustring name, gboolean active) {
	Gtk::TreeModel::Row char_row = *(m_treestore_api->append(api_row.children()));
	char_row[m_columns.row_active] = active;
	char_row[m_columns.keyID] = id;
	char_row[m_columns.vCode] = name;
	char_row[m_columns.is_api] = false;
	char_row[m_columns.keyID_t] = api_row.get_value(m_columns.keyID_t);

	m_api_view->expand_all();
}

} /* namespace Quafe */
