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

#include "application.h"

#include "preferences.h"
#include <boost/bind.hpp>
#include <glibmm/exception.h>
#include <gtkmm/main.h>
#include <gtkmm/togglebutton.h>

namespace Quafe {

// ******************************************************************
// Application class control
Application::Application() :
	app_window(new Window), running(false), m_plugin_current(0) {

	// bind actions before creating the window content
	app_window->action_quit = boost::bind(&Application::quit, this);
	app_window->action_preferences = boost::bind(&Preferences::show_settings, Preferences::instance());
	app_window->m_ptrModulebar->action_plugin_requested = boost::bind(&Application::toggle_plugin, this, _1);

	app_window->create_window();
}

Application::~Application() {
	m_plugin_current = 0;
	PluginInfoList &p_list = Preferences::get_nonconst<PluginInfoList>("plugins");
	PluginInfoList::iterator it = p_list.begin();
	for (; it != p_list.end(); ++it) {
		if((*it).ptr != 0) {
			ustring plugin_info = (*it).ptr->plugin_id();

			(*it).destroy((*it).ptr);
			(*it).ptr = 0;

			LOG(L_NOTICE) << "Plugin '" << plugin_info << "' unloaded.";
		}
	}
	p_list.clear();

	if(app_window) {
		delete app_window;
	}
}

// *******************************************************************
// Application run/quit
void Application::run() {
	Gtk::Main *g_main = Gtk::Main::instance();
	app_window->show_all();
	running = true;

	g_main->run(*app_window);
}

void Application::quit() {
	if(running) {
		running = false;
		LOG(L_NOTICE) << "quafe-etk shutting down...";
		app_window->hide();
	}
}
// *******************************************************************
// Plugin methods
gboolean Application::toggle_plugin(ustring plugin_id) {
	PluginBase *req_plg = 0;
	PluginInfoList p_list = Preferences::get<PluginInfoList>("plugins");
	PluginInfoList::iterator it = p_list.begin();
	LOG(L_DEBUG) << "plugin #1";
	for (; it != p_list.end(); ++it) {
		if ((*it).ptr != 0 && plugin_id.compare((*it).ptr->plugin_id()) == 0) {
			req_plg = (*it).ptr;
			break;
		}
	}

	if (req_plg == 0) {
		LOG(L_ERROR) << "Unknown plugin '" << plugin_id << "' requested";
		return false;
	}

	if (req_plg == m_plugin_current) {
		LOG(L_DEBUG) << "Plugin '" << plugin_id << "' already loaded.";
		return false;
	}

	if (m_plugin_current == 0 || (m_plugin_current && m_plugin_current->close())) {
		m_plugin_current = req_plg;
		app_window->show_plugin_widget(m_plugin_current->show());

		return true;
	}

	return false;
}

void Application::load_plugins() {
	PluginInfoList &p_list = Preferences::get_nonconst<PluginInfoList>("plugins");
	PluginInfoList::iterator it = p_list.begin();
	for (; it != p_list.end(); it++) {
		if((*it).active == false) {
			continue;
		}

		// create plugin and init
		(*it).ptr = (*it).create();
		if((*it).ptr == 0) {
			LOG(L_WARNING) << "failed to initialize plugin";
			continue;
		}

		(*it).plugin_id = (*it).ptr->plugin_id();
		app_window->m_ptrModulebar->add_plugin_button((*it).ptr->plugin_id(), (*it).ptr->plugin_icon_path(), (*it).ptr->plugin_title());

		LOG(L_NOTICE) << "Plugin '" << (*it).ptr->plugin_id() << "' loaded.";
	}
}
}
