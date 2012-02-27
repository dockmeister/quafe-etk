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
#include <pugixml.hpp>

namespace Quafe {

Preferences::Preferences() {
	m_settings["cli.config-path"] = discover_config_path();
	m_settings["cli.plugin-path"] = discover_plugin_path();
	m_settings["minimized"] = (gboolean) false;
	m_settings["fullscreen"] = (gboolean) true;

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
	ustring cfg_path = get<ustring> ("cli.config-path");
	ustring cfg_file = cfg_path + "quafe-etk.cfg";

	if (!dir_exists(cfg_path) || !file_exists(cfg_file)) {
		LOG(L_NOTICE) << "Configuration file not found. Writing default config to: '" << cfg_file << "'";
		return save_config_file();
	}

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(cfg_file.c_str());

	if (!result) {
		LOG(L_WARNING) << "Failed to load configuration: " << result.description();
		return false;
	}

	pugi::xml_node root = doc.document_element();
	pugi::xml_node_iterator it = root.begin();
	for(; it != root.end(); ++it) {
		// process account settings
		if (Regex::match_simple("^accounts$", it->name())) {
			continue;
		}

		// process plugin settings
		if (Regex::match_simple("^plugins$", ustring(it->name()))) {
			pugi::xml_node_iterator pl_it = it->begin();
			for(; pl_it != it->end(); ++pl_it) {
				PluginInfoList &pl_list = get_nonconst<PluginInfoList>("plugins");
				PluginInfoList::iterator search = pl_list.begin();
				for(; search != pl_list.end(); ++search) {
					if((*search).plugin_file.compare(pl_it->child_value()) == 0) {
						(*search).active = (gboolean) pl_it->attribute("active").as_bool();
					}
				}
			}
			continue;
		}

		PreferenceMap::iterator pref = m_settings.find(ustring(it->name()));
		if(pref == m_settings.end()) {
			// unknown preference in xml file
			continue;
		}

		if ((*pref).second.type() == typeid(gboolean))
			(*pref).second = (gboolean) it->attribute("value").as_bool();

		if ((*pref).second.type() == typeid(ustring))
			(*pref).second = (ustring) it->child_value();

	}
	return true;
}

gboolean Preferences::save_config_file() {
	pugi::xml_document doc;
	pugi::xml_node root = doc.append_child("quafe-etk");

	PreferenceMap::const_iterator it = m_settings.begin();
	for (; it != m_settings.end(); ++it) {
		// we dont need cli settings in the config file
		if (Regex::match_simple("^cli", (*it).first))
			continue;

		// process account settings
		if (Regex::match_simple("^accounts$", (*it).first)) {
			pugi::xml_node accounts = root.append_child("accounts");

			AccountInfoList ac_list = boost::any_cast<AccountInfoList>((*it).second);
			AccountInfoList::const_iterator ac_it = ac_list.begin();
			for (; ac_it != ac_list.end(); ++ac_it) {
				pugi::xml_node acc = accounts.append_child("account");

				AccountInfo ac_info = (*ac_it);
				acc.append_attribute("active") = ac_info.active;
				acc.append_child("authid").append_child(pugi::node_pcdata).set_value(ac_info.authid.c_str());
				acc.append_child("authkey").append_child(pugi::node_pcdata).set_value(ac_info.authkey.c_str());

				for (gint i = 0; i < 3; i++) {
					pugi::xml_node chara = acc.append_child("character");
					chara.append_child(pugi::node_pcdata).set_value((ac_info.characters[i].second).c_str());
					chara.append_attribute("active") = ac_info.characters[i].first;
				}
			}

			continue;
		}

		// process plugin settings
		if (Regex::match_simple("^plugins$", (*it).first)) {
			pugi::xml_node plugins = root.append_child("plugins");

			PluginInfoList p_list = boost::any_cast<PluginInfoList>((*it).second);
			PluginInfoList::iterator it = p_list.begin();
			for (; it != p_list.end(); ++it) {
				pugi::xml_node pl = plugins.append_child("plugin");
				pl.append_child(pugi::node_pcdata).set_value(((*it).plugin_file).c_str());
				pl.append_attribute("active") = (*it).active;
			}

			continue;
		}

		// general settings
		if ((*it).second.type() == typeid(gboolean))
			root.append_child((*it).first.c_str()).append_attribute("value") = get<gboolean> ((*it).first);

		if ((*it).second.type() == typeid(ustring))
			root.append_child((*it).first.c_str()).append_child(pugi::node_pcdata).set_value(get<ustring> ((*it).first).c_str());
	}

	ustring cfg_path = get<ustring> ("cli.config-path");
	ustring cfg_file = cfg_path + "quafe-etk.cfg";

	// check if directory exists
	if (!dir_exists(cfg_path)) {
		// try to create directory
		if (!make_dir(cfg_path)) {
			LOG(L_WARNING) << "could not create directory '" << cfg_path << "'";
			return false;
		}
	}

	doc.save_file(cfg_file.c_str());
	return true;
}

void Preferences::parse_plugin_dir() {
	ustring plugin_path = Quafe::Preferences::get<ustring>("cli.plugin-path");

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
		Quafe::PluginInfo p_c = { plugin_name, true, create_plugin, destroy_plugin, 0, "" };
		boost::any_cast<PluginInfoList &>(m_settings["plugins"]).push_back(p_c);
	}

	LOG(L_NOTICE) << "Plugin path: " << plugin_path << " (" << boost::any_cast<PluginInfoList &>(m_settings["plugins"]).size()
			<< " plugins discovered)";
}

// ******************************************************************
// private helper methods
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
