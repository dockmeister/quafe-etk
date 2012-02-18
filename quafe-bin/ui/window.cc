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
#include "../log.h"
#include <boost/bind.hpp>

#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>


namespace Quafe {

Window::Window() {
	set_title("Quafe - eve tool kit");
	m_ptrModulebar = Gtk::manage(new ModuleBar);
}

gboolean Window::create_window() {
	m_refActionGroup = Gtk::ActionGroup::create();
	m_refUIManager = Gtk::UIManager::create();

	signal_hide().connect(on_action_file_quit);


	if (!create_menubar()) {
		LOG(L_ERROR) << "window creation failed: failed to create the menubar";
		return false;
	}

	if (!create_toolbar()) {
		LOG(L_ERROR) << "window creation failed: failed to create the toolbar";
		return false;
	}

	if (!create_modulebar()) {
		LOG(L_ERROR) << "window creation failed: failed to create the pluginbar";
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

gboolean Window::create_menubar() {
	m_refActionGroup->add(Gtk::Action::create("MenuFile", "_File"));
	m_refActionGroup->add(Gtk::Action::create("Quit", Gtk::Stock::QUIT), on_action_file_quit);
	m_refActionGroup->add(Gtk::Action::create("Preferences", Gtk::Stock::PREFERENCES), action_preferences);

	m_refUIManager->insert_action_group(m_refActionGroup);
	add_accel_group(m_refUIManager->get_accel_group());

	ustring ui_info = "<ui>"
		"  <menubar name='Menubar'>"
		"    <menu action='MenuFile'>"
		"      <menuitem action='Quit'/>"
		"    </menu>"
		"  </menubar>"
		"  <toolbar name='Toolbar'>"
		"	 <toolitem action='Quit' />"
		"    <toolitem action='Preferences' />"
		"  </toolbar>"
		"</ui>";

	m_refUIManager->add_ui_from_string(ui_info);

	m_ptrMenubar = static_cast<Gtk::MenuBar*> (m_refUIManager->get_widget("/Menubar"));

	return (m_ptrMenubar != 0);
}

gboolean Window::create_toolbar() {
	m_ptrToolbar = static_cast<Gtk::Toolbar*> (m_refUIManager->get_widget("/Toolbar"));
	m_ptrToolbar->set_show_arrow(false);
	m_ptrToolbar->set_toolbar_style(Gtk::TOOLBAR_ICONS);

	return (m_ptrToolbar != 0);
}
gboolean Window::create_modulebar() {


	return true;
}

Window::~Window() {
	// since gtk is managing all widgets nothing to do here
}

}
