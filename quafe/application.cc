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
#include <quafe-logging.h>

#include "application.h"

#include "pluginmanager.h"
#include "preferences.h"
#include "ui/applicationwindow.h"
#include "plugins/plugininterface.h"

#include <boost/bind.hpp>
#include <gtkmm/main.h>
#include <glibmm/miscutils.h>

QUAFE_DECLARE_STATIC_LOGGER("Quafe");

namespace Quafe {

// ******************************************************************
// Application class control
Application::Application() :
		running(false), m_plugin_current(0) {

	Glib::ustring data_dir = Preferences::get<Glib::ustring>("data-directory");

	//[ Using glade to create main window and connecting signals
	Glib::ustring glade_file = Glib::build_filename(data_dir, "ui", "applicationwindow.glade");
	Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create_from_file(glade_file);

	refBuilder->get_widget_derived("ApplicationWindow", app_window);

	app_window->signal_hide().connect(boost::bind(&Application::quit, this));
	app_window->signal_plugin_enabled(boost::bind(&Application::on_plugin_enabled, this, _1));
	app_window->signal_plugin_enabled(boost::bind(&Application::on_plugin_disabled, this, _1));
	app_window->signal_plugin_activated(boost::bind(&Application::on_plugin_activated, this, _1));
	app_window->signal_plugin_deactivated(boost::bind(&Application::on_plugin_deactivated, this, _1));

	//]

	Preferences::instance()->create_dialog(*app_window);

	//[ add valid plugins to gui (active or not)
	PluginInfoList p_list = PluginManager::instance()->get_plugin_list();
	PluginInfoList::iterator it = p_list.begin();

	for (; it != p_list.end(); ++it) {
		PluginInfo info = *it;
		if (!info.validate())
			continue;

		Glib::ustring icon_path = Glib::build_filename(Preferences::get<Glib::ustring>("data-directory"), "images", info.icon);
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
		LOG_INFO("quafe-etk shutting down...");
		app_window->hide();
	}
}

// *******************************************************************
// Plugin methods
bool Application::on_plugin_enabled(const Glib::ustring &id) {
	PluginInfo info;
	if (!PluginManager::instance()->find(id, info)) {
		LOG_ERROR("Unable to find plugin '%1'.", id);
		return false;
	}

	PluginManager::instance()->destroy(info);

	return true;
}

bool Application::on_plugin_disabled(const Glib::ustring &id) {
	PluginInfo info;
	if (!PluginManager::instance()->find(id, info)) {
		LOG_ERROR("Unable to find plugin '%1'.", id);
		return false;
	}

	PluginManager::instance()->create(info);

	return true;
}

bool Application::on_plugin_activated(const Glib::ustring &id) {
	PluginInterface *plugin = 0;
	if (!PluginManager::instance()->find(id, plugin)) {
		LOG_ERROR("Unknown plugin '%1' requested.", id);
		return false;
	}

	app_window->show_plugin_widget(plugin->show());

	return true;
}

bool Application::on_plugin_deactivated(const Glib::ustring &id) {
	PluginInterface *plugin = 0;
	if (!PluginManager::instance()->find(id, plugin)) {
		LOG_ERROR("Unknown plugin '%1' requested.", id);
		return false;
	}

	if(!plugin || !plugin->close()) {
		LOG_ERROR("Failed to close plugin '%1'.", id);
		return false;
	}

	return true;
}

const ApplicationWindow * Application::get_window() {
	return app_window;
}
}
