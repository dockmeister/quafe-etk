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


#include "window.h"

#include "modulebar.h"

#include <boost/bind.hpp>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/checkmenuitem.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>


namespace Quafe {

ApplicationWindow::ApplicationWindow() :
	m_ptrModulebar(new ModuleBar) {
	set_title("Quafe - eve tool kit");

}

ApplicationWindow::~ApplicationWindow() {
	m_refContentFrame.remove();

	QUAFE_DELETE(m_ptrModulebar);
}

gboolean ApplicationWindow::create_window() {
	m_refActionGroup = Gtk::ActionGroup::create();
	m_refUIManager = Gtk::UIManager::create();

	signal_hide().connect(action_quit);


	if (!create_menubar()) {
		LOG(L_ERROR) << "window creation failed: failed to create the menubar";
		return false;
	}

	if (!create_toolbar()) {
		LOG(L_ERROR) << "window creation failed: failed to create the toolbar";
		return false;
	}

	Gtk::VBox *vbox = Gtk::manage(new Gtk::VBox);
	Gtk::HBox *hbox = Gtk::manage(new Gtk::HBox);

	vbox->pack_start(*m_ptrMenubar, Gtk::PACK_SHRINK);
	vbox->pack_start(*m_ptrToolbar, Gtk::PACK_SHRINK);
	vbox->pack_start(*hbox, Gtk::PACK_EXPAND_WIDGET);
	hbox->pack_start(*m_ptrModulebar, Gtk::PACK_SHRINK);
	hbox->pack_start(m_refContentFrame, Gtk::PACK_EXPAND_WIDGET);

	add(*vbox);

	return true;
}

gboolean ApplicationWindow::create_menubar() {
	m_refActionGroup->add(Gtk::Action::create("MenuFile", "_File"));
	m_refActionGroup->add(Gtk::Action::create("Quit", Gtk::Stock::QUIT), action_quit);
	m_refActionGroup->add(Gtk::Action::create("Preferences", Gtk::Stock::PREFERENCES), action_preferences);
	m_refActionGroup->add(Gtk::Action::create("MenuPlugins", "_Plugins"));
	m_refActionGroup->add(Gtk::Action::create("AllPlugins", "_Plugins"));

	m_refUIManager->insert_action_group(m_refActionGroup);
	add_accel_group(m_refUIManager->get_accel_group());

	ustring ui_info = "<ui>"
		"  <menubar name='Menubar'>"
		"    <menu action='MenuFile'>"
		"      <menuitem action='Quit'/>"
		"    </menu>"
		"    <menuitem action='MenuPlugins' />"
		"  </menubar>"
		"  <toolbar name='Toolbar'>"
		"	 <toolitem action='Quit' />"
		"    <toolitem action='Preferences' />"
		"  </toolbar>"
		"</ui>";

	m_refUIManager->add_ui_from_string(ui_info);

	m_ptrMenubar = static_cast<Gtk::MenuBar*> (m_refUIManager->get_widget("/Menubar"));
	m_ptrPluginMenu = static_cast<Gtk::MenuItem*> (m_refUIManager->get_widget("/Menubar/MenuPlugins"));

	Gtk::Menu *m = Gtk::manage(new Gtk::Menu);
	m_ptrPluginMenu->set_submenu(*m);
	return (m_ptrMenubar != 0);
}

gboolean ApplicationWindow::create_toolbar() {
	m_ptrToolbar = static_cast<Gtk::Toolbar*> (m_refUIManager->get_widget("/Toolbar"));
	m_ptrToolbar->set_show_arrow(false);
	m_ptrToolbar->set_toolbar_style(Gtk::TOOLBAR_ICONS);

	return (m_ptrToolbar != 0);
}

void ApplicationWindow::add_plugin(ustring plugin_id, ustring plugin_title, ustring plugin_icon, bool active) {
	Gtk::CheckMenuItem *item = Gtk::manage(new Gtk::CheckMenuItem(plugin_title));
	item->set_active(active);
	item->signal_toggled().connect(boost::bind(&ApplicationWindow::menu_item_plugin, this, item, plugin_id));


	m_ptrPluginMenu->get_submenu()->append(*item);

	m_ptrModulebar->add_plugin_button(plugin_id, plugin_icon, plugin_title);
}

void ApplicationWindow::menu_item_plugin(Gtk::CheckMenuItem *item, ustring plugin_id) {
	action_endisable_plugin(item->get_active(), plugin_id);

	if(item->get_active()) {
		m_ptrModulebar->button_list[plugin_id].button->show();
		m_ptrModulebar->button_list[plugin_id].label->show();
	} else {
		m_ptrModulebar->button_list[plugin_id].button->hide();
		m_ptrModulebar->button_list[plugin_id].label->hide();
	}
}

gboolean ApplicationWindow::show_plugin_widget(Gtk::Widget &widget) {
	m_refContentFrame.remove();
	m_refContentFrame.add(widget);
	m_refContentFrame.show_all_children(true);

	set_focus(m_refContentFrame);
	return true;
}

}
