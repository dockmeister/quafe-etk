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

#include "preferences.h"

#include <pwd.h>
#include <iostream>
#include <fstream>
#include <dlfcn.h>
#include <glibmm/fileutils.h>
#include <glibmm/regex.h>

namespace Quafe {

Preferences::Preferences() {
	m_settings["cli.config-path"] = discover_config_path();
	m_settings["cli.plugin-path"] = discover_plugin_path();
	m_settings["general.minimized"] = false;
	m_settings["general.fullscreen"] = true;

	AccountInfoList ac_list;
	m_settings["accounts"] = ac_list;

	PluginInfoList pl_list;
	m_settings["plugins"] = pl_list;
}

Preferences::~Preferences() {
}

void Preferences::show_settings() {
	sett_window.show();
}

// ******************************************************************
// methods to load and save settings
gboolean Preferences::parse_command_line(int argc, char **argv) {

	return false;
}

gboolean Preferences::parse_config_file() {
	return false;
}

gboolean Preferences::save_config_file() {
	using namespace boost::property_tree;
	ptree pt;

	PreferenceMap::const_iterator it = m_settings.begin();
	for(; it != m_settings.end(); ++it) {
		// we dont need cli settings in the config file
		if(Regex::match_simple("^cli", (*it).first))
			continue;

		// process account settings
		if(Regex::match_simple("^accounts$", (*it).first)) {
			AccountInfoList ac_list = boost::any_cast<AccountInfoList>((*it).second);
			AccountInfoList::const_iterator ac_it = ac_list.begin();
			for(; ac_it != ac_list.end(); ++ac_it) {
				ptree &node = pt.add("quafe-etk.accounts.account", "");
				AccountInfo ac_info = (*ac_it);
				node.add("<xmlattr>.active", ac_info.active);
				node.add("authid", ac_info.authid);
				node.add("authkey", ac_info.authkey);
				node.add("character", ac_info.characters[0].first).add("<xmlattr>.active", ac_info.characters[0].second);
				node.add("character", ac_info.characters[1].first).add("<xmlattr>.active", ac_info.characters[1].second);
				node.add("character", ac_info.characters[2].first).add("<xmlattr>.active", ac_info.characters[2].second);
			}
		}

		// process plugin settings
		if(Regex::match_simple("^plugins$", (*it).first)) {

		}
	}

	return false;
}

// ******************************************************************
// private helper methods
/*!\brief
 *
 * \param plugin_list
 */
void Preferences::discover_plugins() {
	ustring plugin_path = Quafe::Preferences::get<Glib::ustring>("plugin-path");

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
		Quafe::PluginContainer p_c = { create_plugin, destroy_plugin };
		plugin_list.push_back(p_c);
	}

	LOG(L_NOTICE) << "Plugin path: " << plugin_path << " (" << plugin_list.size() << " plugins discovered)";
}

ustring Preferences::discover_config_path() {
	ustring def_cfgdir = "/tmp";
	uid_t user_id = geteuid();
	struct passwd* user_info = getpwuid(user_id);

	if (user_info && user_info->pw_dir) {
		try {
			Dir home_dir(user_info->pw_dir);
			def_cfgdir = user_info->pw_dir;
		} catch (FileError &fe) {
			LOG(L_WARNING) << fe.what();
		}
	} else {
		LOG(L_WARNING) << "Using /tmp as config dir. Any configuration will be lost!";
	}

	return def_cfgdir + "/.quafe-etk/";
}

ustring Preferences::discover_plugin_path() {
	return ustring(QUAFE_DATADIR) + "plugins/";
}
} /* namespace Quafe */
