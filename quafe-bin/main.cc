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

#include <quafe-etk.h>

#include "utility.h"
#include "application.h"
#include "preferences.h"

/*!\brief
 *
 */
int main(int argc, char **argv) {
	Glib::init();
	Gtk::Main kit(argc, argv);

	Quafe::Preferences * settings = Quafe::Preferences::instance();
	if (settings->parse_command_line(argc, argv)) {
		return EXIT_SUCCESS;
	}

	LOG(L_NOTICE) << "quafe-etk starting up...";

	settings->parse_plugin_dir();
	settings->parse_config_file();

	try {
		Quafe::Application * app = Quafe::Application::instance();
		app->load_plugins();
		app->run();
	} catch (std::exception &e) {
		LOG(L_CRITICAL) << e.what();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
