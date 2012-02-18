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

#include <dlfcn.h>
#include <glibmm/fileutils.h>
#include <glibmm/regex.h>
#include <gtkmm.h>

#include "log.h"
#include "application.h"
#include "plugin/pluginbase.h"


int discover_plugins(Quafe::PluginList &plugin_list) {
	// run configuration in eclipse should be the git work tree
	// post-build-step copies the plugins  to data/plugins
	// With autotools it should be replaced with PREFIX/share/quafe-etk/plugins
	ustring plugin_path = "data/plugins/";

	try {
		Dir plugin_dir(plugin_path);

		Dir::const_iterator it = plugin_dir.begin();

		for (; it != plugin_dir.end(); ++it) {
			ustring plugin_name = *it;

			if (!Regex::match_simple("^libquafe-[a-z]+.so$", plugin_name))
				continue;

			void* plugin = dlopen((plugin_path + *it).c_str(), RTLD_LAZY);
			if (!plugin) {
				LOG(L_ERROR) << "Cannot load plugin " << dlerror();
				continue;
			}

			Quafe::create_t* create_plugin = (Quafe::create_t*) dlsym(plugin, "create");
			const char* dlsym_error = dlerror();
			if (dlsym_error) {
				LOG(L_ERROR) << "Cannot load symbol create: " << dlsym_error;
				continue;
			}

			Quafe::destroy_t* destroy_plugin = (Quafe::destroy_t*) dlsym(plugin, "destroy");
			dlsym_error = dlerror();
			if (dlsym_error) {
				LOG(L_ERROR) << "Cannot load symbol destroy: " << dlsym_error;
				continue;
			}

			// plugin successfully loaded, push to plugin list
			Quafe::PluginContainer p_c = {create_plugin, destroy_plugin};
			plugin_list.push_back(p_c);
		}

		LOG(L_NOTICE) << "Plugin path: " << plugin_path << " (" << plugin_list.size() << " plugins discovered)";
	} catch (FileError &fe) {
		LOG(L_ERROR) << fe.what();

		return 0;
	}

	return 1;
}

int main(int argc, char **argv) {
	LOG(L_NOTICE) << "quafe-etk starting up...";
	Gtk::Main kit(argc, argv);
	Quafe::Application app;
	discover_plugins(app.plugins_getlist());
	app.plugins_create();

	app.run(kit);
	//app.plugins_destroy();
	return EXIT_SUCCESS;
}
