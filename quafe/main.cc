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

#include <quafe-config.h>
#include <quafe-logging.h>

#include "preferences.h"
#include "pluginmanager.h"
#include "application.h"

#include <eapi/exception.h>
#include <eapi/eapi.h>
#include <eapi/sheets/keyinfo.h>

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <glibmm/miscutils.h>

QUAFE_DECLARE_STATIC_LOGGER("Quafe");

/*!\brief
 *
 */
int main(int argc, char **argv) {
	Gtk::Main kit(argc, argv);

	if (Quafe::Preferences::init(argc, argv)) {
		return EXIT_SUCCESS;
	}

	LOG_INFO("Starting quafe-etk. (version: %1, release: %2)", QUAFE_VERSION, QUAFE_BUILD_RELEASE);
	{

		Glib::ustring directory = Glib::build_filename(Quafe::Preferences::get<Glib::ustring>("data-directory"), "plugins");
		Quafe::PluginManager::init(directory);

		Glib::ustring eapi_dir = Quafe::Preferences::get<Glib::ustring>("config-directory");
		if (!EAPI::Main::init(eapi_dir)) {
			return EXIT_FAILURE;
		}

		Quafe::Preferences::instance()->parse_account_list();
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

	Quafe::Preferences::instance()->save_config_file();

	return EXIT_SUCCESS;
}
