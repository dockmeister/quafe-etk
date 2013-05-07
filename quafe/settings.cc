/*
 * settings.cc
 *
 *  Created on: Apr 23, 2013
 *      Author: cn
 */
#include <quafe-logging.h>
#include "settings.h"
#include "utils/exception.h"

#include <boost/property_tree/xml_parser.hpp>

#include <glibmm/fileutils.h>
#include <glibmm/miscutils.h>
#include <giomm/file.h>

#include <iostream>

namespace Quafe {
QUAFE_DECLARE_STATIC_LOGGER("Quafe");
boost::optional<UstringTranslator::external_type> UstringTranslator::get_value(UstringTranslator::internal_type const &v) {
	return Glib::ustring(v);
}

boost::optional<UstringTranslator::internal_type> UstringTranslator::put_value(UstringTranslator::external_type const &v) {
	return v.raw();
}
}

namespace boost {
	namespace property_tree {
template<>
struct translator_between<std::string, Glib::ustring> {
	typedef Quafe::UstringTranslator type;
};
	}
}
namespace Quafe {

boost::property_tree::ptree Settings::pt;
Glib::ustring Settings::c_data_directory;
Glib::ustring Settings::c_config_directory;

#if defined(WIN32)
const char Settings::q_config_subdir[] = "Quafe";
#else
const char Settings::q_config_subdir[] = ".quafe";
#endif

using namespace boost::property_tree;

Settings::Settings(int argc, char **argv)
	: print_version(false), print_help(false) {
	//[ parsing command line arguments
	for(uint16_t i = 1; i < (argc-1); i++) {
		Glib::ustring arg(argv[i]);
		if(arg == "-h" || arg == "--help") {
			print_help = true;
			return;
		}

		if(arg == "-v" || arg == "--version") {
			print_version = true;
			return;
		}

		if(arg.find("--data-directory") > 0) {
			c_data_directory = arg.replace(0, arg.find('=')+1, "");
			LOG_DEBUG("--data-directory found. value: %1", c_data_directory);
		}

		if(arg.find("--config-directory") > 0) {
			c_config_directory = arg.replace(0, arg.find('=')+1, "");
			LOG_DEBUG("--config-directory found. value: %1", c_config_directory);
		}
	}
	//] parsing command line arguments

	//[ lookup config directory
	if(c_config_directory.empty()) {
		c_config_directory = Glib::build_filename(Glib::get_home_dir(), Glib::ustring(q_config_subdir));
	}

	if(!Glib::file_test(c_config_directory, Glib::FILE_TEST_EXISTS | Glib::FILE_TEST_IS_DIR)) {
		LOG_INFO("Configuration directory not found - creating %1", c_config_directory);
		Glib::RefPtr<Gio::File> cdir = Gio::File::create_for_path(c_config_directory);

		try {
			cdir->make_directory();
		} catch(Glib::Error &e) {
			LOG_WARN("Failed to create directory '%1': %2", c_config_directory, e.what());
			LOG_WARN("Using temporary folder. Any settings or changes will be lost. (%1)", Glib::get_tmp_dir());
			c_config_directory = Glib::get_tmp_dir();
		}
	}

#if QUAFE_HAVE_LOG4CXX
	Glib::ustring log_dir = Glib::build_filename(c_config_directory, "logs");
	if(!Glib::file_test(log_dir, Glib::FILE_TEST_EXISTS | Glib::FILE_TEST_IS_DIR)) {
		Glib::RefPtr<Gio::File> cdir = Gio::File::create_for_path(log_dir);

		try {
			cdir->make_directory();
		} catch(Glib::Error &e) {
		}
	}
#endif

	Glib::ustring eapi_dir = Glib::build_filename(c_config_directory, "eveapi");
	if(!Glib::file_test(log_dir, Glib::FILE_TEST_EXISTS | Glib::FILE_TEST_IS_DIR)) {
		Glib::RefPtr<Gio::File> cdir = Gio::File::create_for_path(eapi_dir);

		try {
			cdir->make_directory();
		} catch(Glib::Error &e) {
		}
	}
	//] lookup config directory

	//[ lookup data directory
	std::vector<Glib::ustring> dir_hints;
	if(c_data_directory.empty()) {
		dir_hints.push_back(QUAFE_DATADIR);
		dir_hints.push_back(QUAFE_PREFIX);
		dir_hints.push_back(Glib::get_current_dir());
	} else {
		dir_hints.push_back(c_data_directory);
	}

	std::vector<Glib::ustring>::const_iterator it;
	for(it = dir_hints.begin(); it != dir_hints.end(); it++) {
		if(Glib::file_test(Glib::build_filename(*it, "media", "quafe.png"), Glib::FILE_TEST_EXISTS)
			&& Glib::file_test(Glib::build_filename(*it, "plugins"), Glib::FILE_TEST_EXISTS | Glib::FILE_TEST_IS_DIR)
			&& Glib::file_test(Glib::build_filename(*it, "ui", "applicationwindow.glade"), Glib::FILE_TEST_EXISTS)) {

			c_data_directory = *it;
			break;
		}
	}

	if(it == dir_hints.end()) {
		throw Exception("Unable to locate data-directory. Use --data-directory to specify the quafe folder with plugins and images.");
	}
	//] lookup data directory
}

Settings::Settings()
	: print_version(false), print_help(false){
}

Settings::~Settings() {

}

bool Settings::process_command_line() {
	if(print_version) {
		std::cout << std::boolalpha;
		std::cout << QUAFE_VERSION << " release mode: " << (bool)QUAFE_BUILD_RELEASE<< std::endl;
	}

	if(print_help) {
		std::cout << std::boolalpha;
		std::cout << "-h, --help	print this help" << std::endl;
		std::cout << "-v, --version	print Quafe version" << std::endl;
		std::cout << "--data-directory=DIR	specify the data directory. (default: "<< Glib::get_user_config_dir() << ")" << std::endl;
		std::cout << "--config-directory=DIR	specify the config directory. (default: " << QUAFE_DATADIR << ")" << std::endl;
	}

	return (print_version || print_help);
}

void Settings::load(const Glib::ustring filename) {
	read_xml(filename.raw(), pt);

	const ptree &pl = pt.get_child("plugins");
	for(ptree::const_iterator it = pl.begin(); it != pl.end(); it++) {
		const ptree::value_type &v = *it;
		PluginInfo pinfo;
		if(pinfo.load(v.second)) {
			m_plugin_list.push_back(pinfo);
		}
	}

	const ptree &ac = pt.get_child("accounts");
	for(ptree::const_iterator it = ac.begin(); it != ac.end(); it++) {
		const ptree::value_type &v = *it;
		AccountInfo ainfo;
		if(ainfo.load(v.second)) {
			m_account_list.push_back(ainfo);
		}
	}
}

void Settings::save(const Glib::ustring filename) {
	pt.put("plugins", ""); // remove all previous plugins
	for(PluginInfo::const_iterator it = m_plugin_list.begin(); it != m_plugin_list.end(); it++) {
		ptree &pl = pt.add("plugins.plugin", "");
		(*it).save(pl);
	}
}

template <class T>
T Settings::get(const Glib::ustring path, const T default_value) {
	T return_value = default_value;
	try {
		return_value = pt.get<T>(path.raw());
	} catch(ptree_bad_data &e) {
		LOG_WARN("Wrong access to %1 as %2. This is a bug in quafe.", path, typeid(T).name());
	} catch(ptree_bad_path &e) {
		LOG_TRACE("Accessing %1 the first time. Storing and return the default value: %2", path, default_value);
		pt.put(path.raw(), default_value);
	}

	return return_value;
}

Glib::ustring Settings::get_string(const Glib::ustring path, const Glib::ustring default_value) {
	return get<Glib::ustring>(path, default_value);
}

bool Settings::get_boolean(const Glib::ustring path, const bool default_value) {
	return get<bool>(path, default_value);
}

int32_t Settings::get_integer(const Glib::ustring path, const int32_t default_value) {
	return get<int32_t>(path, default_value);
}

/****************************************************************************************************
 *
 */
bool PluginInfo::load(const ptree &pt) {
	try {
		file = pt.get<Glib::ustring>("filename");
		id = pt.get<Glib::ustring>("id");
		title = pt.get<Glib::ustring>("title");
		icon = pt.get<Glib::ustring>("icon");
		found = false;
	} catch(ptree_error &e) {
		LOG_WARN("Malformed config file. Unable to load plugin info.");
		return false;
	}

	return true;
}

bool PluginInfo::save(ptree &pt) const {
	pt.put("filename", file);
	pt.put("id", id);
	pt.put("title", title);
	pt.put("icon", icon);

	return true;
}

bool AccountInfo::load(const ptree &pt) {
	try {
		active = pt.get<bool>("active");
		authid = pt.get<uint32_t>("authid");
		authkey = pt.get<Glib::ustring>("authkey");

		const ptree &chars = pt.get_child("characters");
		for(ptree::const_iterator it = chars.begin(); it != chars.end(); it++) {
			const ptree::value_type &v = *it;
			CharacterInfo cinfo;
			if(v.first == "character" && cinfo.load(v.second)) {
				characters.push_back(cinfo);
			}
		}
	} catch(ptree_error &e) {
		LOG_WARN("Malformed config file. Unable to load account info.");
		return false;
	}

	return true;
}

bool AccountInfo::save(ptree &pt) const {
	pt.put("active", active);
	pt.put("authid", authid);
	pt.put("authkey", authkey);

	ptree &chars = pt.put("characters", "");
	for(CharacterInfo::const_iterator it = characters.begin(); it != characters.end(); it++) {
		ptree &c = chars.add("characters.character", "");
		(*it).save(c);
	}
	return true;
}

bool CharacterInfo::load(const ptree &pt) {
	try {
		active = pt.get<bool>("active");
		id = pt.get<uint32_t>("id");
		name = pt.get<Glib::ustring>("name");
	} catch(ptree_error &e) {
		LOG_WARN("Malformed config file. Unable to load character info.");
		return false;
	}

	return true;
}

bool CharacterInfo::save(ptree &pt) const {
	pt.put("active", active);
	pt.put("id", id);
	pt.put("name", name);

	return true;
}
} /* namespace Quafe */
