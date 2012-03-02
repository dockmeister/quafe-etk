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


#ifndef WINDOW_H_
#define WINDOW_H_

#include <quafe-etk.h>
#include <utility.h>

#include <boost/function.hpp>
#include <gtkmm/window.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/actiongroup.h>
#include <gtkmm/action.h>
#include <gtkmm/stock.h>
#include <gtkmm/menu.h>
#include <gtkmm/menubar.h>
#include <gtkmm/checkmenuitem.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/scrolledwindow.h>

namespace Quafe {
class ModuleBar;
/*!\brief
 *
 */
class ApplicationWindow : public Gtk::Window {
	friend class Application;
protected:
	ApplicationWindow();
	~ApplicationWindow();

	gboolean create_window();
	gboolean show_plugin_widget(Gtk::Widget &widget);

	void add_plugin(ustring plugin_id, ustring plugin_title, ustring plugin_icon,  bool active);

	// boost functions
	boost::function<void ()> action_quit;
	boost::function<void ()> action_preferences;
	boost::function<void (bool active, ustring)> action_endisable_plugin;

	void menu_item_plugin(Gtk::CheckMenuItem *item, ustring plugin_id);
private:
	gboolean create_menubar();
	gboolean create_toolbar();
	gboolean create_modulebar();

	RefPtr<Gtk::UIManager> m_refUIManager;
	RefPtr<Gtk::ActionGroup> m_refActionGroup;

	Gtk::MenuBar *m_ptrMenubar;
	Gtk::MenuItem *m_ptrPluginMenu; //< used to add menuitems 'plugins'
	Gtk::Toolbar *m_ptrToolbar;
	Gtk::ScrolledWindow m_refContentFrame;
	ModuleBar *m_ptrModulebar;


};

}

#endif /* WINDOW_H_ */
