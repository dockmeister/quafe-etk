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

#include <quafe-etk.h>

#include "preferences.h"
#include "pluginmanager.h"
#include "utility.h"
#include "application.h"
#include <eapi/eapi.h>
#include <eapi/sheets/keyinfo.h>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH
#undef TEST_PLUGIN_MANAGER

/*!\brief
 *
 */
int main(int argc, char **argv) {
	if(Quafe::Preferences::init(argc, argv)) {
		return EXIT_SUCCESS;
	}

	Gtk::Main kit(argc, argv);

	ustring directory = Quafe::Preferences::get<ustring>("plugin-directory");
	Quafe::PluginManager::init(directory);

	{
		ustring eapi_dir = Quafe::Preferences::get<ustring>("eapi-directory");
		if(!EAPI::Main::init(eapi_dir)) {
			return EXIT_FAILURE;
		}

		Quafe::AccountInfoList acclist = Quafe::Preferences::get<Quafe::AccountInfoList>("accouts");
		foreach(Quafe::AccountInfo info, acclist) {
			int id = boost::lexical_cast<int>(info.authid);
			EAPI::KeyInfo::create(id, info.authkey);
		}
	}

	try {
		Quafe::Application * app = Quafe::Application::instance();
		app->run();
	} catch (std::exception &e) {
		LOG(L_CRITICAL) << e.what();
		return EXIT_FAILURE;
	}

	Quafe::Preferences::instance()->save_config_file();

	return EXIT_SUCCESS;
}
