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

#include <quafe-etk.h>

#include <boost/function.hpp>
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treemodelcolumn.h>

namespace Quafe {

/*< */
class ApiModel : public Gtk::TreeModel::ColumnRecord {
public:
	Gtk::TreeModelColumn<bool> row_active;
	Gtk::TreeModelColumn<Glib::ustring> auth_id;
	Gtk::TreeModelColumn<Glib::ustring> auth_key;
	Gtk::TreeModelColumn<Glib::ustring> name;

	ApiModel() {
		add(row_active);
		add(auth_id);
		add(auth_key);
		add(name);
	}
};

/*!\brief
 *
 */
class PreferenceDialog : public Gtk::Dialog {
	friend class Preferences;
public:
	PreferenceDialog(const ustring &title, Gtk::Window &parent, bool modal);
	virtual ~PreferenceDialog();

protected:
	void insert_api_row(Gtk::TreeModel::Row &row, gboolean active, ustring auth_id, ustring auth_key, ustring name);
	virtual void on_response(int response);
	boost::function<void (ustring id, ustring key, API_CHANGE chg)> action_add_api_key;

	Gtk::TreeView m_TreeView;
	ApiModel m_columns;
	Glib::RefPtr<Gtk::TreeStore> m_refTreeModel;

private:
	Gtk::Widget *create_general_tab();
	Gtk::Widget *create_account_tab();

	void add_api_button_clicked(API_CHANGE e);

	Gtk::Entry *m_entry_auth_id, *m_entry_auth_key;

};

} /* namespace Quafe */
#endif /* WINDOW_PREFERENCES_H_ */
