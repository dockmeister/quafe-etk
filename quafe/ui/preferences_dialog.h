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

#ifndef WINDOW_PREFERENCES_H_
#define WINDOW_PREFERENCES_H_

#include <boost/signals2.hpp>
#include <gtkmm/builder.h>
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treemodelcolumn.h>

namespace Quafe {

/*< */
class ApiModel: public Gtk::TreeModel::ColumnRecord {
public:
	Gtk::TreeModelColumn<bool> row_active;
	Gtk::TreeModelColumn<Glib::ustring> keyID;
	Gtk::TreeModelColumn<Glib::ustring> vCode;
	Gtk::TreeModelColumn<bool> is_api;
	Gtk::TreeModelColumn<Glib::ustring> keyID_t;

	ApiModel() {
		add(row_active);
		add(keyID);
		add(vCode);
		add(is_api);
		add(keyID_t);
	}
};

/*!\brief
 *
 */
class PreferenceDialog: public Gtk::Dialog {
	friend class Preferences;
public:
	typedef boost::signals2::signal<void(const Glib::ustring &keyID, const Glib::ustring &vCode)> signal_api_t;
	typedef boost::signals2::signal<bool(const Glib::ustring &keyID, const Glib::ustring &cid, bool active)> signal_toogle_t;

	PreferenceDialog(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
	virtual ~PreferenceDialog();

	void set_parent(Gtk::Window &parent);

	inline boost::signals2::connection signal_api_added(const signal_api_t::slot_type &slot) {
		return m_signal_api_add.connect(slot);
	}

	inline boost::signals2::connection signal_api_deleted(const signal_api_t::slot_type &slot) {
		return m_signal_api_delete.connect(slot);
	}

	inline boost::signals2::connection signal_character_toggled(const signal_toogle_t::slot_type &slot) {
		return m_signal_toggle_character.connect(slot);
	}

protected:
	Gtk::TreeModel::Row insert_api_row(Glib::ustring keyID, Glib::ustring vCode, gboolean active);
	void insert_character_row(Gtk::TreeModel::Row &api_row, Glib::ustring id, Glib::ustring name, gboolean active);
	virtual void on_response(int response);

	//Gtk::TreeView m_TreeView;
	//
	//Glib::RefPtr<Gtk::TreeStore> m_refTreeModel;

private:
	void on_api_add_button_clicked();
	void on_api_check_toggled(const Glib::ustring &tree_path);

	Gtk::Dialog *md_add_api_key;
	Gtk::Button *mb_api_add, *mb_api_delete;
	Gtk::Entry *m_entry_auth_id, *m_entry_auth_key;

	ApiModel m_columns;
	Glib::RefPtr<Gtk::TreeStore> m_treestore_api;
	Glib::RefPtr<Gtk::CellRendererToggle> m_refToggle;

	Gtk::TreeView *m_api_view;
	Gtk::TreeSelection *m_api_selection;
	Gtk::TreeViewColumn *m_api_view_column_toggle, *m_api_view_column_keyid, *m_api_view_column_vcode;
	Gtk::CellRendererToggle *m_api_view_renderer_toggle;
	Gtk::CellRendererText *m_api_view_renderer_keyid, *m_api_view_renderer_vcode;

	signal_api_t m_signal_api_add;
	signal_api_t m_signal_api_delete;
	signal_toogle_t m_signal_toggle_character;

};

} /* namespace Quafe */
#endif /* WINDOW_PREFERENCES_H_ */
