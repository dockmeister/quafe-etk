/*
 * main.cc
 *
 *  Created on: 07.02.2012
 *      Author: cn
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
