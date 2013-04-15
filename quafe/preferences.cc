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

#include "preferences.h"
#include "preferences_translator.hpp"

#include "application.h"
#include "pluginmanager.h"
#include "plugins/plugininterface.h"
#include "ui/dialogs.h"
#include "ui/preferences_dialog.h"
#include "ui/applicationwindow.h"

#include <eapi/eapi.h>
#include <eapi/sheets/keyinfo.h>
#include <pwd.h>
#include <iostream>
#include <boost/bind.hpp>
#include <glibmm/fileutils.h>
#include <glibmm/regex.h>
#include <gtkmm/builder.h>
#include <glibmm/miscutils.h>
#include <boost/lexical_cast.hpp>

QUAFE_DECLARE_STATIC_LOGGER("EAPI");

namespace Quafe {

bool Preferences::init(int argc, char **argv) {
	Preferences *pref = Preferences::instance();

	if (pref->parse_command_line(argc, argv)) {
		return true;
	}

	pref->parse_config_file();

	//[ Checking for valid directories
	if (!Glib::file_test(get<Glib::ustring>("data-directory"), Glib::FILE_TEST_IS_DIR)) {
		LOG_WARN("Data directory doesnt exists or is not writable : '" << get<Glib::ustring>("data-directory") << "'");
		return true;
	}
	//]

	return false;
}

Preferences::Preferences() :
	opt_cli("Command line arguments"), opt_general("quafe-etk"), m_dialog(0) {

	m_config_path = default_config_path();

	Glib::ustring dir_data = Glib::ustring(QUAFE_DATADIR);
	Glib::ustring dir_plugins = Glib::build_filename(dir_data, "plugins");
	Glib::ustring dir_eapi = m_config_path;

	opt_cli.add_options()
			("help,h", "print this message")
			("version,v", "")
			("config-directory", po::value(&m_config_path)->default_value(m_config_path), "Set the path where the configuration and any Eve API related file are stored")
			("data-directory", po::value<Glib::ustring>()->default_value(dir_data), "Set the path where EAPI resources are stored")
			("minimize,m ", po::value<bool>()->default_value(false), "Start quafe-etk minimized")
			("silent,s", po::value<bool>()->default_value(false), "");

	opt_general.add_options()
			("maximized", po::value<bool>()->default_value(false))
			("size-width", po::value<int>()->default_value(800))
			("size-height", po::value<int>()->default_value(600))
			("test-value", po::value<bool>()->default_value(false), "test-value");

	opt_all.add(opt_cli).add(opt_general);
}

Preferences::~Preferences() {
}

void Preferences::show_settings() {
	if(m_dialog == 0) {
		throw Exception("Dialog is not initialized");
	}

	int response = m_dialog->run();

	//[ apply and save settings
	if (response == Gtk::RESPONSE_APPLY) {
		save_config_file();
	}
	//]
}

void Preferences::create_dialog(Gtk::Window &window) {
	Glib::ustring glade_file = Glib::build_filename(get<Glib::ustring>("data-directory"), "ui", "preferences.glade");

	if(!Glib::file_test(glade_file, Glib::FILE_TEST_EXISTS)) {
		throw Exception("Could not locate Gladefile: " + glade_file);
	}

	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(glade_file);

	builder->get_widget_derived("DialogPreferences", m_dialog);

	m_dialog->set_parent(window);
	m_dialog->signal_api_added(boost::bind(&Preferences::on_api_add, this, _1, _2));
	m_dialog->signal_api_deleted(boost::bind(&Preferences::on_delete_api, this, _1));
	m_dialog->signal_character_toggled(boost::bind(&Preferences::on_api_character_toggled, this, _1, _2, _3));


	//[ insert account info into dialog treestore
	for (EAPI::KeyInfo::const_iterator it = EAPI::KeyInfo::begin(); it != EAPI::KeyInfo::end(); ++it) {
		EAPI::KeyInfo *key = (*it);
		Gtk::TreeModel::Row api_row = m_dialog->insert_api_row(key->value<Glib::ustring>("keyID"), key->value<Glib::ustring>("vCode"), true);

		EAPI::KeyInfo::CharacterList c_list = key->get_character_list();
		EAPI::KeyInfo::CharacterList::const_iterator ct = c_list.begin();
		for(; ct != c_list.end(); ++ct) {
			m_dialog->insert_character_row(api_row, boost::lexical_cast<Glib::ustring>((*ct).id), (*ct).name, (*ct).active);
		}
	}

//	EAPI::KeyInfo::begin();
//	AccountInfoList::iterator it = m_account_list.begin();
//	for (; it != m_account_list.end(); ++it) {
//		AccountInfo acc_info = *it;
//		Gtk::TreeModel::Row row = *(m_dialog->m_refTreeModel->append());
//		m_dialog->insert_api_row(row, acc_info.active, acc_info.authid, acc_info.authkey, Glib::ustring(""));
//
//		Gtk::TreeModel::Row child = *(m_dialog->m_refTreeModel->append(row.children()));
//		m_dialog->insert_api_row(child, acc_info.characters[0].first, Glib::ustring(""), Glib::ustring(""), acc_info.characters[0].second);
//
//		child = *(m_dialog->m_refTreeModel->append(row.children()));
//		m_dialog->insert_api_row(child, acc_info.characters[1].first, Glib::ustring(""), Glib::ustring(""), acc_info.characters[1].second);
//
//		child = *(m_dialog->m_refTreeModel->append(row.children()));
//		m_dialog->insert_api_row(child, acc_info.characters[2].first, Glib::ustring(""), Glib::ustring(""), acc_info.characters[2].second);
//	}
//	//]

}

void Preferences::on_api_add(const Glib::ustring &keyID, const Glib::ustring &vCode) {
	EAPI::KeyInfo *m_key;
	try {
		int ikeyID = boost::lexical_cast<int>(keyID);
		m_key = EAPI::KeyInfo::create(ikeyID, vCode);
	} catch (EAPI::Exception &e) {
		show_error_dialog("Failed to add API Key.", e.what());
		return;
	} catch(...) {
		show_error_dialog("Failed to add API Key.", "The keyID is not a valid number");
		return;
	}

	try {
		m_key->update(boost::bind(&Preferences::on_api_add_callback, this, _1));
	} catch(EAPI::NotOutdatedException &e) {
		// api is still up to date (most likely from cache)
		on_api_add_callback(m_key);
		return;
	} catch (EAPI::Exception &e) {
		LOG_DEBUG(e.what());
		return;
	}

	m_dialog->set_sensitive(false);
}

void Preferences::on_api_add_callback(EAPI::KeyInfo *key) {
	m_dialog->set_sensitive(true);
	LOG_DEBUG("callback recieved!");

	if(key->check_status(EAPI::API_STATUS_ERROR) > 0) {
		show_error_dialog("Failed to add API Key.", key->m_error);
		EAPI::KeyInfo::remove(key);
		return;
	}

	//EAPI::KeyInfo::List char_list = key->get_character_list();
	//EAPI::KeyInfo::const_iterator it = key->get_character_list().begin();
}

void Preferences::on_delete_api(const Glib::ustring &keyID) {

}

bool Preferences::on_api_character_toggled(const Glib::ustring &keyID, const Glib::ustring &cID, bool active) {
	int keyid_t;
	int cid_t;
	try {
		keyid_t = boost::lexical_cast<int>(keyID);
		cid_t = boost::lexical_cast<int>(cID);
	} catch(...) {
		return false;
	}

	EAPI::KeyInfo *key;
	if(!EAPI::KeyInfo::find(keyid_t, key)) {
		return false;
	}

	if(!key->find_character(cid_t)) {
		return false;
	}

	key->set_character_active(cid_t, active);
	return true;
}

//void Preferences::apply_api_changes(const Glib::ustring &keyID, const Glib::ustring &vCode, API_CHANGE chg) {
//	if (chg == API_ADD) {
//		//[ TODO check if api is valid and fetch characters (make it sensitive/no async here)
//		EAPI::KeyInfo *m_key = 0;
//		try {
//			int ikeyID = boost::lexical_cast<int>(keyID);
//			m_key = EAPI::KeyInfo::create(ikeyID, vCode);
//		} catch (EAPI::Exception &e) {
//			show_error_dialog("Failed to add API Key.", e.what());
//			return;
//		} catch(...) {
//			show_error_dialog("Failed to add API Key.", "The keyID is not a valid number");
//			return;
//		}
//		//]
//
//		// make sensitive
//		//m_key->update_sync();
//		// ? abort
//		// insert key
//
//		//[ insert AccountInfo into settings
//		AccountInfo acc_info = { true, keyID, vCode };
//		acc_info.characters[0] = std::make_pair<gboolean, Glib::ustring>(true, "TestChar1");
//		acc_info.characters[1] = std::make_pair<gboolean, Glib::ustring>(false, "TestChar2");
//		acc_info.characters[2] = std::make_pair<gboolean, Glib::ustring>(false, "TestChar3");
//
//		m_account_list.push_back(acc_info);
//		//]
//
//		//[ apply changes to the Dialog
//		PreferenceDialog *dialog = static_cast<PreferenceDialog *> (m_dialog);
//		Gtk::TreeModel::Row row = *(dialog->m_refTreeModel->append());
//		dialog->insert_api_row(row, acc_info.active, acc_info.authid, acc_info.authkey, Glib::ustring(""));
//
//		Gtk::TreeModel::Row child = *(dialog->m_refTreeModel->append(row.children()));
//		dialog->insert_api_row(child, acc_info.characters[0].first, Glib::ustring(""), Glib::ustring(""), acc_info.characters[0].second);
//
//		child = *(dialog->m_refTreeModel->append(row.children()));
//		dialog->insert_api_row(child, acc_info.characters[1].first, Glib::ustring(""), Glib::ustring(""), acc_info.characters[1].second);
//
//		child = *(dialog->m_refTreeModel->append(row.children()));
//		dialog->insert_api_row(child, acc_info.characters[2].first, Glib::ustring(""), Glib::ustring(""), acc_info.characters[2].second);
//		//]
//
//		return;
//	}
//
//	if (chg == API_DELETE) {
//		show_error_dialog("Function not supported yet", "Delete the Account manually by editing the config file.");
//		return;
//	}
//}

// ******************************************************************
// methods to load and save settings
gboolean Preferences::parse_command_line(int argc, char **argv) {
	// TODO register directory validator to ensure paths are absolute and not relative
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
	Glib::ustring cfg_file = build_filename(m_config_path, "quafe-etk.cfg");

	if (!Glib::file_test(cfg_file, Glib::FILE_TEST_EXISTS)) {
		LOG_INFO("Configuration file not found. Writing default config to: '" << cfg_file << "'");
		return save_config_file();
	}

	pugi::xml_parse_result result = doc.load_file(cfg_file.c_str());

	if (!result) {
		LOG_WARN("Failed to load configuration: " << result.description());
		return false;
	}
	//]

	//
	po::parsed_options parsed(&opt_all);

	// TODO unknown options throw a boost:exception
	//[ iterate over config file, check if its a valid preferences and apply
	pugi::xml_node root = doc.document_element();
	pugi::xml_node_iterator it = root.begin();
	for (; it != root.end(); ++it) {
		//[ save account list to temporary
		//[ EAPI is not initialized yet
		if (Glib::Regex::match_simple("^account-list$", it->name())) {
			continue;
		}
		//]

		//[ Parse plugin preferences
		if (Glib::Regex::match_simple("^plugin-list$", it->name())) {
			PluginInfoList plugin_list = PluginManager::instance()->get_plugin_list();
			translate<PluginInfoList>::from(*it, plugin_list);
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

void Preferences::parse_account_list() {
	EAPI::KeyInfo *key = 0;

	pugi::xml_node account_list = doc.document_element().child("account-list");
	if(account_list.empty()) {
		return;
	}

	pugi::xml_node_iterator it = account_list.begin();
	for(; it != account_list.end(); ++it) {
		if(Glib::Regex::match_simple("^account$", it->name())) {
			translate<EAPI::KeyInfo>::from(*it, *key);
		} else
			LOG_WARN("unknown xml option found within account-list");
	}

}

gboolean Preferences::save_config_file() {
	doc.reset();
	pugi::xml_node root = doc.append_child("quafe-etk");

	//[ iterate over General options
	po::variables_map::const_iterator it = vmap.begin();
	for (; it != vmap.end(); ++it) {
		const Glib::ustring pref_name = (*it).first;
		const po::variable_value pref_value = (*it).second;

		// create a new xml_node
		pugi::xml_node node = root.append_child(pref_name.c_str());

		//[ try to translate the variable_value to a xml_node
		if (pref_value.value().type() == typeid(bool)) {
			translate<bool>::to(node, pref_value);
			continue;
		}

		if (pref_value.value().type() == typeid(Glib::ustring)) {
			translate<Glib::ustring>::to(node, pref_value);
			continue;
		}

		if (pref_value.value().type() == typeid(int)) {
			translate<int>::to(node, pref_value);
			continue;
		}

		// no translator found!
		LOG_DEBUG("No translator found for preference: '" << pref_name << "'");
		//]
	}
	//]

	//[ Parsing window settings
	if(Application::is_initialized()) {
		const ApplicationWindow *win = Application::instance()->get_window();

		if(!win->is_maximized()) {
			int w, h;
			win->get_size(w, h);
			root.child("size-width").attribute("value") = w;
			root.child("size-height").attribute("value") = h;
		} else {
			root.child("maximized").attribute("value") = true;
		}
	}
	//]

	//[ Add plugin-list and account-list since they are not stored in vmap
	pugi::xml_node pl_node = root.append_child("plugin-list");
	PluginInfoList plugin_list = PluginManager::instance()->get_plugin_list();
	translate<PluginInfoList>::to(pl_node, plugin_list);

	pugi::xml_node ac_node = root.append_child("account-list");
	for(EAPI::KeyInfo::iterator it = EAPI::KeyInfo::begin(); it != EAPI::KeyInfo::end(); it++) {
		const EAPI::KeyInfo *key = (*it);
		translate<EAPI::KeyInfo>::to(ac_node, key);
	}

	//]

	//[ finally save the config file
	Glib::ustring cfg_file = Glib::build_filename(m_config_path, "quafe-etk.cfg");

	// check if directory exists
	if (!Glib::file_test(m_config_path, Glib::FILE_TEST_IS_DIR)) {
		//TODO: try to create directory
		//if (!Gio::make_dir(m_config_path)) {
			LOG_WARN("could not create directory '" << m_config_path << "'");
			return false;
		//}
	}

	doc.save_file(cfg_file.c_str());
	//]

	return true;
}

// ******************************************************************
// private helper methods

Glib::ustring Preferences::default_config_path() {
	Glib::ustring def_cfgdir = "/tmp";
	uid_t user_id = geteuid();
	// FIXME memory leak
	struct passwd* user_info = getpwuid(user_id);
	// TODO switch to Glib::get_config_dir() ||
	if (user_info && user_info->pw_dir) {
		try {
			Glib::Dir home_dir(user_info->pw_dir);
			def_cfgdir = user_info->pw_dir;
		} catch (Glib::FileError &fe) {
			LOG_WARN(fe.what());
		}
	} else {
		LOG_WARN("Using /tmp as config dir. Any configuration will be lost!");
	}

	return def_cfgdir + "/.quafe-etk/";
}

} /* namespace Quafe */
