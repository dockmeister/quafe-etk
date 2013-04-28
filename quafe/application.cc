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

#include "utils/exception.h"
#include "settings.h"
#include "pluginmanager.h"
#include "ui/applicationwindow.h"
#include "plugins/plugininterface.h"

#include <eapi/eapi.h>
#include <eapi/sheets/keyinfo.h>

#include <boost/bind.hpp>
#include <gtkmm/main.h>
#include <gtkmm-3.0/gtkmm/main.h>
#include <glibmm/miscutils.h>

QUAFE_DECLARE_STATIC_LOGGER("Quafe");

namespace Quafe {

// ******************************************************************
// Application class control
Application::Application()
	: running(false), m_plugin_current(0) {

	mPluginManager = new PluginManager();

	Settings settings;
	//[ Using glade to create main window and connecting signals
	Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create_from_file(settings.get_glade_directory("applicationwindow.glade"));

	refBuilder->get_widget_derived("ApplicationWindow", app_window);

	app_window->signal_hide().connect(boost::bind(&Application::quit, this));
	app_window->signal_plugin_enabled(boost::bind(static_cast<bool (PluginManager::*)(const Glib::ustring&)>(&PluginManager::open), mPluginManager, _1));
	app_window->signal_plugin_disabled(boost::bind(static_cast<bool (PluginManager::*)(const Glib::ustring&)>(&PluginManager::close), mPluginManager, _1));
	app_window->signal_plugin_activated(boost::bind(&Application::on_plugin_activated, this, _1));
	app_window->signal_plugin_deactivated(boost::bind(&Application::on_plugin_deactivated, this, _1));

	//]

	//Preferences::instance()->create_dialog(*app_window);

	//[ open plugins and add valid plugins to gui (active or not)
	mPluginManager->open_all();

	PluginInfo::List p_list = settings.get_plugin_list();
	for (PluginInfo::const_iterator it = p_list.begin(); it != p_list.end(); ++it) {
		app_window->add_plugin((*it).id, (*it).title, (*it).icon, (*it).autoload);
	}
	//]
}

Application::~Application() {
	m_plugin_current = 0;
	QUAFE_DELETE(mPluginManager);
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
bool Application::on_plugin_activated(const Glib::ustring &id) {
	PluginInterface *plugin = 0;
	if (!mPluginManager->find(id, plugin)) {
		LOG_ERROR("Unknown plugin '%1' requested.", id);
		return false;
	}

	app_window->show_plugin_widget(plugin->show());

	return true;
}

bool Application::on_plugin_deactivated(const Glib::ustring &id) {
	PluginInterface *plugin = 0;
	if (!mPluginManager->find(id, plugin)) {
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


/**********************************************************************************************
 *
 */

#if QUAFE_HAVE_LOG4CXX
#	include <log4cxx/patternlayout.h>
#	include <log4cxx/fileappender.h>
#	include <log4cxx/consoleappender.h>
#endif

int main(int argc, char **argv) {
	using namespace Quafe;
	using namespace log4cxx;

	Gtk::Main kit(argc, argv);

#if QUAFE_HAVE_LOG4CXX
	//[ setup log system
	PatternLayoutPtr p_layout = new PatternLayout("%r [%-5p] %c - %m%n");
    ConsoleAppenderPtr c_appender = new ConsoleAppender(p_layout);
    Logger::getRootLogger()->addAppender(c_appender);
	//]
#endif

	Settings settings(argc, argv);
	if(settings.process_command_line()) {
		return EXIT_SUCCESS;
	}

#if QUAFE_HAVE_LOG4CXX
	Glib::ustring qlog_file = settings.get_log_directory("quafe.log");
	FileAppenderPtr f_appender = new FileAppender(p_layout, qlog_file.raw());
	Logger::getLogger("Quafe")->addAppender(f_appender);
#endif

#if EAPI_HAVE_LOG4CXX
	Glib::ustring elog_file = settings.get_log_directory("eapi.log");
	FileAppenderPtr fe_appender = new FileAppender(p_layout, elog_file.raw());
	Logger::getLogger("EAPI")->addAppender(fe_appender);
#endif


	LOG_INFO("Starting quafe-etk. (version: %1, release: %2)", QUAFE_VERSION, QUAFE_BUILD_RELEASE);
	{
		if (!EAPI::Main::init(settings.get_eapi_directory())) {
			return EXIT_FAILURE;
		}

		AccountInfo::List ac_list = settings.get_account_list();
		for(AccountInfo::const_iterator it = ac_list.begin(); it != ac_list.end(); it++) {
			const AccountInfo &ac_info = *it;

			if(ac_info.active == false) {
				continue;
			}

			EAPI::KeyInfo *key = 0;
			try {
				key = EAPI::KeyInfo::create(ac_info.authid, ac_info.authkey);

			} catch(EAPI::Exception &e) {
				LOG_WARN("Failed to add KeyInfo: ", e.what());
				continue;
			}

			for(CharacterInfo::const_iterator it = ac_info.characters.begin(); it != ac_info.characters.end(); it++) {
				const CharacterInfo &c_info = *it;
				key->set_character_active(c_info.id, c_info.active);
			}
		}
	}

	try {
		Quafe::Application * app = Quafe::Application::instance();
		app->run();
	} catch (Quafe::Exception &e) {
		LOG_FATAL("Exception: %1", e.what());
		return EXIT_FAILURE;
	} catch (std::exception &e) {
		LOG_FATAL("Exception: %1", e.what());
		return EXIT_FAILURE;
	}

	//Quafe::Preferences::instance()->save_config_file();

	return EXIT_SUCCESS;
}
