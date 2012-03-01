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

	//
	Preferences::instance()->create_dialog(*app_window);
}

Application::~Application() {
	m_plugin_current = 0;
	unload_all_plugins();

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
	PluginInfoList &p_list = Preferences::instance()->get_plugin_list();

	PluginInfoList::iterator it = std::find_if(p_list.begin(), p_list.end(), boost::bind(&PluginInfo::id, _1) == plugin_id);

	if (it == p_list.end()) {
		LOG(L_ERROR) << "Unknown plugin '" << plugin_id << "' requested";
		return false;
	}

	if ((*it).ptr == m_plugin_current) {
		LOG(L_DEBUG) << "Plugin '" << plugin_id << "' already loaded.";
		return false;
	}

	if (m_plugin_current == 0 || (m_plugin_current && m_plugin_current->close())) {
		m_plugin_current = (*it).ptr;
		app_window->show_plugin_widget(m_plugin_current->show());

		return true;
	}

	return false;
}

void Application::endisable_plugin(bool active, ustring plugin_id) {
	PluginInfoList &p_list = Preferences::instance()->get_plugin_list();
	PluginInfoList::iterator it = std::find_if(p_list.begin(), p_list.end(), boost::bind(&PluginInfo::id, _1) == plugin_id);

	if(active && it != p_list.end()) {
		load_plugin(*it);

	} else if(!active && it != p_list.end()) {
		unload_plugin(*it);
	}
}

void Application::load_all_plugins() {
	PluginInfoList &p_list = Preferences::instance()->get_plugin_list();
	PluginInfoList::iterator it = p_list.begin();
	for(; it != p_list.end(); ++it) {
		PluginInfo p_info = *it;
		load_plugin(p_info);

		ustring icon_path = Preferences::get<ustring>("data-path") + p_info.params.icon;
		app_window->add_plugin(p_info.id, p_info.params.title, icon_path, p_info.active);
	}
}

void Application::load_plugin(PluginInfo &p_info) {


	p_info.ptr = p_info.create();
	if (p_info.ptr == 0) {
		LOG(L_WARNING) << "failed to initialize plugin";
		return;
	}

	LOG(L_NOTICE) << "Plugin '" << p_info.params.id << "' loaded.";
}

void Application::unload_all_plugins() {
	PluginInfoList &p_list = Preferences::instance()->get_plugin_list();

	std::for_each(p_list.begin(), p_list.end(), boost::bind(&Application::unload_plugin, this, _1));
}

void Application::unload_plugin(PluginInfo &p_info) {
	if (p_info.ptr == 0)
		return;

	p_info.destroy(p_info.ptr);
	p_info.ptr = 0;

	LOG(L_NOTICE) << "Plugin '" << p_info.params.id << "' unloaded.";
}

}
