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
#include "preferences_translator.h"

#include "eapi/keyinfo.h"
#include "include/pluginbase.h"
#include "ui/dialogs.h"
#include "ui/dialog_preferences.h"
#include "utility.h"

#include <pwd.h>
#include <iostream>
#include <fstream>
#include <dlfcn.h>
#include <boost/bind.hpp>
#include <glibmm/fileutils.h>
#include <glibmm/regex.h>
#include <pugixml.hpp>

namespace Quafe {

bool Preferences::init(int argc, char **argv) {
	Preferences *pref = Preferences::instance();

	if(pref->parse_command_line(argc, argv)) {
		return true;
	}

	pref->parse_plugin_dir();
	pref->parse_config_file();

	return false;
}

Preferences::Preferences() :
	opt_cli("Command line arguments"), opt_general("quafe-etk") {

	opt_cli.add_options()("help,h", "print this message")("version,v", "")("config-path",
			po::value<ustring>()->default_value(discover_config_path()),
			"Set the path where the configuration and any Eve API related file are stored")("data-path",
			po::value<ustring>()->default_value(discover_plugin_path()), "Set the path where plugins and resources are stored")("minimize,m ",
			po::value<bool>()->default_value(false), "Start quafe-etk minimized")("silent,s", po::value<bool>()->default_value(false), "");

	opt_general.add_options()("test-value", po::value<bool>()->default_value(false), "test-value");

	opt_all.add(opt_cli).add(opt_general);
}

Preferences::~Preferences() {
}

void Preferences::show_settings() {
	int response = m_dialog->run();

	//[ apply and save settings
	if (response == Gtk::RESPONSE_APPLY) {
		save_config_file();
	}
	//]
}

void Preferences::create_dialog(Gtk::Window &window) {
	m_dialog = new PreferenceDialog("Preferences", window, true);
	m_dialog->action_add_api_key = boost::bind(&Preferences::apply_api_changes, this, _1, _2, _3);

	//[ insert account info into dialog treestore
	AccountInfoList::iterator it = m_account_list.begin();
	for (; it != m_account_list.end(); ++it) {
		AccountInfo acc_info = *it;
		Gtk::TreeModel::Row row = *(m_dialog->m_refTreeModel->append());
		m_dialog->insert_api_row(row, acc_info.active, acc_info.authid, acc_info.authkey, ustring(""));

		Gtk::TreeModel::Row child = *(m_dialog->m_refTreeModel->append(row.children()));
		m_dialog->insert_api_row(child, acc_info.characters[0].first, ustring(""), ustring(""), acc_info.characters[0].second);

		child = *(m_dialog->m_refTreeModel->append(row.children()));
		m_dialog->insert_api_row(child, acc_info.characters[1].first, ustring(""), ustring(""), acc_info.characters[1].second);

		child = *(m_dialog->m_refTreeModel->append(row.children()));
		m_dialog->insert_api_row(child, acc_info.characters[2].first, ustring(""), ustring(""), acc_info.characters[2].second);
	}
	//]

}

void Preferences::apply_api_changes(const ustring &keyID, const ustring &vCode, API_CHANGE chg) {
	if (chg == API_ADD) {
		//[ TODO check if api is valid and fetch characters (make it sensitive/no async here)
		EAPI::KeyInfo *m_key;
		try {
			m_key = EAPI::KeyInfo::create(keyID, vCode);
		} catch(EAPI::Exception &e) {
			show_error_dialog("Failed to add API Key.", e.what());
			return;
		}
		//]

		//[ insert AccountInfo into settings
		AccountInfo acc_info = { true, keyID, vCode };
		acc_info.characters[0] = std::make_pair<gboolean, ustring>(true, "TestChar1");
		acc_info.characters[1] = std::make_pair<gboolean, ustring>(false, "TestChar2");
		acc_info.characters[2] = std::make_pair<gboolean, ustring>(false, "TestChar3");

		m_account_list.push_back(acc_info);
		//]

		//[ apply changes to the Dialog
		PreferenceDialog *dialog = static_cast<PreferenceDialog *> (m_dialog);
		Gtk::TreeModel::Row row = *(dialog->m_refTreeModel->append());
		dialog->insert_api_row(row, acc_info.active, acc_info.authid, acc_info.authkey, ustring(""));

		Gtk::TreeModel::Row child = *(dialog->m_refTreeModel->append(row.children()));
		dialog->insert_api_row(child, acc_info.characters[0].first, ustring(""), ustring(""), acc_info.characters[0].second);

		child = *(dialog->m_refTreeModel->append(row.children()));
		dialog->insert_api_row(child, acc_info.characters[1].first, ustring(""), ustring(""), acc_info.characters[1].second);

		child = *(dialog->m_refTreeModel->append(row.children()));
		dialog->insert_api_row(child, acc_info.characters[2].first, ustring(""), ustring(""), acc_info.characters[2].second);
		//]

		return;
	}

	if (chg == API_DELETE) {
		show_error_dialog("Function not supported yet", "Delete the Account manually by editing the config file.");
		return;
	}
}

// ******************************************************************
// methods to load and save settings
gboolean Preferences::parse_command_line(int argc, char **argv) {
	po::store(po::parse_command_line(argc, argv, opt_all), vmap);
	po::notify(vmap);

	if (vmap.count("help")) {
		std::cout << opt_cli << std::endl;
		return true;
	}

	if (vmap.count("version")) {
		std::cout << QUAFE_STRING << std::endl;
		return true;
	}
	return false;
}

gboolean Preferences::parse_config_file() {
	//[ load or create configfile
	ustring cfg_path = get<ustring> ("config-path");
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
	//]

	//
	po::parsed_options parsed(&opt_all);

	//[ iterate over config file, check if its a valid preferences and apply
	pugi::xml_node root = doc.document_element();
	pugi::xml_node_iterator it = root.begin();
	for (; it != root.end(); ++it) {
		//[ Parse account settings
		if (Regex::match_simple("^account-list$", it->name())) {
			translate<AccountInfoList>::from(*it, m_account_list);
			continue;
		}
		//]

		//[ Parse plugin preferences
		if (Regex::match_simple("^plugin-list$", it->name())) {
			translate<PluginInfoList>::from(*it, m_plugin_list);
			continue;
		}
		//]

		//[ All other preferences
		po::option opt;
		translate<po::option>::from(*it, opt);

		parsed.options.push_back(opt);
		//]
	}
	//]

	//[ let boost::program_options parsing the result
	po::store(parsed, vmap);
	po::notify(vmap);
	//]

	return true;
}

gboolean Preferences::save_config_file() {
	pugi::xml_document doc;
	pugi::xml_node root = doc.append_child("quafe-etk");

	//[ iterate over General options
	po::variables_map::const_iterator it = vmap.begin();
	for (; it != vmap.end(); ++it) {
		const ustring pref_name = (*it).first;
		const po::variable_value pref_value = (*it).second;

		// create a new xml_node
		pugi::xml_node node = root.append_child(pref_name.c_str());

		//[ try to translate the variable_value to a xml_node
		if (pref_value.value().type() == typeid(bool)) {
			translate<bool>::to(node, pref_value);
			continue;
		}

		if (pref_value.value().type() == typeid(ustring)) {
			translate<ustring>::to(node, pref_value);
			continue;
		}

		// no translator found!
		LOG(L_DEBUG) << "No translator found for preference: '" << pref_name << "'";
		//]
	}
	//]

	//[ Add plugin-list and account-list since they are not stored in vmap
	pugi::xml_node pl_node = root.append_child("plugin-list");
	translate<PluginInfoList>::to(pl_node, m_plugin_list);

	pugi::xml_node ac_node = root.append_child("account-list");
	translate<AccountInfoList>::to(ac_node, m_account_list);
	//]

	//[ finally save the config file
	ustring cfg_path = get<ustring> ("config-path");
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
	//]

	return true;
}
// ******************************************************************
// plugin methods

void Preferences::parse_plugin_dir() {
	ustring plugin_path = get<ustring> ("data-path") + "plugins/";

	Dir plugin_dir(plugin_path);
	Dir::const_iterator it = plugin_dir.begin();

	for (; it != plugin_dir.end(); ++it) {
		ustring plugin_name = *it;

		if (!Regex::match_simple("^libquafe-[a-z]+.so$", plugin_name))
			continue;

		Quafe::PluginInfo pl_info;
		pl_info.plugin_file = plugin_name;
		pl_info.active = true;

		if (open_plugin(pl_info)) {
			m_plugin_list.push_back(pl_info);
		}
	}

	LOG(L_NOTICE) << "Plugin path: " << plugin_path << " (" << m_plugin_list.size() << " plugins discovered)";
}

gboolean Preferences::open_plugin(PluginInfo &pl_info) {
	ustring plugin_path = get<ustring> ("data-path") + "plugins/";

	void* plugin = dlopen((plugin_path + pl_info.plugin_file).c_str(), RTLD_LAZY);
	if (!plugin) {
		LOG(L_ERROR) << "Cannot load plugin " << dlerror();
		return false;
	}

	Quafe::create_t* create_plugin = (Quafe::create_t*) dlsym(plugin, "create");
	const char* dlsym_error = dlerror();
	if (dlsym_error) {
		LOG(L_ERROR) << "Cannot load symbol create: " << dlsym_error;
		return false;
	}

	Quafe::destroy_t* destroy_plugin = (Quafe::destroy_t*) dlsym(plugin, "destroy");
	dlsym_error = dlerror();
	if (dlsym_error) {
		LOG(L_ERROR) << "Cannot load symbol destroy: " << dlsym_error;
		return false;
	}

	Quafe::get_params_t* plugin_params = (Quafe::get_params_t*) dlsym(plugin, "get_params");
	dlsym_error = dlerror();
	if (dlsym_error) {
		LOG(L_ERROR) << "Cannot load symbol get_params: " << dlsym_error;
		return false;
	}

	pl_info.dlhandle = plugin;
	pl_info.create = create_plugin;
	pl_info.destroy = destroy_plugin;
	pl_info.params = plugin_params();
	pl_info.id = pl_info.params.id;

	return true;
}

gboolean Preferences::close_plugin(PluginInfo &pl_info) {
	if (pl_info.dlhandle == 0 || pl_info.ptr) {
		LOG(L_WARNING) << "Failed to close '" << pl_info.plugin_file << "'";
		return false;
	}

	if (dlclose(pl_info.dlhandle)) {
		// dlclose != 0 : error occured
		LOG(L_WARNING) << dlerror();
		return false;
	}

	pl_info.dlhandle = 0;
	return true;
}

// ******************************************************************
// private helper methods

ustring Preferences::discover_config_path() {
	ustring def_cfgdir = "/tmp";
	uid_t user_id = geteuid();
	struct passwd* user_info = getpwuid(user_id);

	if (user_info && user_info->pw_dir) {
		//@todo: change to dir_exists();
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
	return ustring(QUAFE_DATADIR);
}
} /* namespace Quafe */
