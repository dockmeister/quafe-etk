/**
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

#include "applicationwindow.h"

#include "pluginbar.h"

#include "../preferences.h"
#include <boost/bind.hpp>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/checkmenuitem.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <glibmm/miscutils.h>

namespace Quafe {

ApplicationWindow::ApplicationWindow(BaseObjectType *cobject, Glib::RefPtr<Gtk::Builder> refBuilder)
	: Gtk::Window(cobject), m_refBuilder(refBuilder), maximized(false) {

	data_dir = Preferences::get<Glib::ustring>("data-directory");

	//[ apply additional window options
	set_icon_from_file(Glib::build_filename(data_dir, "images", "quafe.png"));

	if (Preferences::get<bool>("maximized") == true) {
		maximize();
		maximized = true;
	} else {
		resize(Preferences::get<int>("size-width"), Preferences::get<int>("size-height"));
	}
	//]

	m_refBuilder->get_widget_derived("pluginbar", m_pluginbar);
	m_refBuilder->get_widget("pluginframe", m_pluginframe);
	m_pluginframe->remove(); // remove all childs from pluginframe;

	m_refBuilder->get_widget("menuitem_plugins", m_pluginMenu);
}

ApplicationWindow::~ApplicationWindow() {
	m_pluginframe->remove();

	//QUAFE_DELETE(m_pluginbar); Gtk takes care of it
}

void ApplicationWindow::add_plugin(Glib::ustring plugin_id, Glib::ustring plugin_title, Glib::ustring plugin_icon, bool active) {
	Gtk::CheckMenuItem *item = Gtk::manage(new Gtk::CheckMenuItem(plugin_title));
	item->set_active(active);
	item->signal_toggled().connect(boost::bind(&ApplicationWindow::on_menuitem_plugin, this, item, plugin_id));

	m_pluginMenu->get_submenu()->append(*item);

	m_pluginbar->add_plugin_button(plugin_id, plugin_icon, plugin_title);
}

void ApplicationWindow::on_menuitem_plugin(Gtk::CheckMenuItem *item, Glib::ustring plugin_id) {
	if(item->get_active()) {
		if(m_signal_plugin_enabled(plugin_id)) {
			m_pluginbar->show_plugin_button(plugin_id);

		} else {
			item->set_active(false);
		}

		return;
	}

	if(!item->get_active()) {
		if(m_signal_plugin_disabled(plugin_id)) {
			m_pluginbar->hide_plugin_button(plugin_id);

		} else {
			item->set_active(true);
		}

		return;
	}
}

void ApplicationWindow::show_plugin_widget(Gtk::Widget &widget) {
	m_pluginframe->remove();
	m_pluginframe->add(widget);
	m_pluginframe->show_all_children(true);

	set_focus(*m_pluginframe);
}

bool ApplicationWindow::on_window_state_event(GdkEventWindowState *event) {
	if ((event->changed_mask & Gdk::WINDOW_STATE_MAXIMIZED) == Gdk::WINDOW_STATE_MAXIMIZED) {
		if ((event->new_window_state & Gdk::WINDOW_STATE_MAXIMIZED) == Gdk::WINDOW_STATE_MAXIMIZED) {
			maximized = true;
		} else {
			maximized = false;
		}
	}
	return true;
}

}
