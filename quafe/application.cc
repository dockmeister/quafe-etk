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

#include "application.h"

#include "pluginmanager.h"
#include "preferences.h"
#include "ui/window.h"
#include "ui/modulebar.h"
#include <boost/bind.hpp>
#include <gtkmm/main.h>

namespace Quafe {

// ******************************************************************
// Application class control
Application::Application() :
	app_window(new ApplicationWindow), running(false), m_plugin_current(0) {

	//[ bind actions and create the Application window
	app_window->action_quit = boost::bind(&Application::quit, this);
	app_window->action_preferences = boost::bind(&Preferences::show_settings, Preferences::instance());
	app_window->action_endisable_plugin = boost::bind(&Application::endisable_plugin, this, _1, _2);
	app_window->m_ptrModulebar->action_plugin_requested = boost::bind(&Application::toggle_plugin, this, _1);

	app_window->create_window();
	//]

	Preferences::instance()->create_dialog(*app_window);

	//[ add valid plugins to gui (active or not)
	PluginInfoList p_list = PluginManager::instance()->get_plugin_list();
	PluginInfoList::iterator it = p_list.begin();

	for(; it != p_list.end(); ++it) {
		PluginInfo info = *it;
		if(!info.validate())
			continue;

		ustring icon_path = build_filename(Preferences::get<ustring>("data-directory"), "images", info.icon);
		app_window->add_plugin(info.id, info.title, icon_path, info.active);
	}
	//]
}


Application::~Application() {
	m_plugin_current = 0;

	QUAFE_DELETE(app_window);
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
	if (running) {
		running = false;
		LOG(L_NOTICE) << "quafe-etk shutting down...";
		app_window->hide();
	}
}


// *******************************************************************
// Plugin methods

gboolean Application::toggle_plugin(ustring plugin_id) {
	PluginBase *plugin = 0;

	if (!PluginManager::instance()->find(plugin_id, plugin)) {
		LOG(L_ERROR) << "Unknown plugin '" << plugin_id << "' requested";
		return false;
	}

	if (plugin == m_plugin_current) {
		LOG(L_DEBUG) << "Plugin '" << plugin_id << "' already loaded.";
		return false;
	}

	if (m_plugin_current == 0 || (m_plugin_current && m_plugin_current->close())) {
		m_plugin_current = plugin;
		app_window->show_plugin_widget(m_plugin_current->show());

		return true;
	}

	return false;
}

void Application::endisable_plugin(bool active, ustring plugin_id) {
	PluginInfo info;
	if(!PluginManager::instance()->find(plugin_id, info))
		return;

	// FIXME seg fault at toggeling twice
	if(active) {
		PluginManager::instance()->create(info);
	} else {
		PluginManager::instance()->destroy(info);
	}
}

const ApplicationWindow * Application::get_window() {
	return app_window;
}
}
