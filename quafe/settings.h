/*
 * settings.h
 *
 *  Created on: Apr 23, 2013
 *      Author: cn
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <quafe-config.h>

#include <boost/property_tree/ptree.hpp>

#include <glibmm/ustring.h>
#include <glibmm/miscutils.h>

namespace Quafe {

/*!
 *
 */
struct PluginInfo {
	typedef std::vector<PluginInfo> List;
	typedef std::vector<PluginInfo>::iterator iterator;
	typedef std::vector<PluginInfo>::const_iterator const_iterator;

	bool autoload;
	Glib::ustring  file;
	Glib::ustring  id;
	Glib::ustring  title;
	Glib::ustring  icon;

	bool found;

	bool load(const boost::property_tree::ptree &pt);
	bool save(boost::property_tree::ptree &pt) const;
};

/*!
 *
 */
struct CharacterInfo {
	typedef std::vector<CharacterInfo> List;
	typedef std::vector<CharacterInfo>::iterator iterator;
	typedef std::vector<CharacterInfo>::const_iterator const_iterator;

	bool active;
	uint32_t id;
	Glib::ustring name;

	bool load(const boost::property_tree::ptree &pt);
	bool save(boost::property_tree::ptree &pt) const;
};

/*!
 *
 */
struct AccountInfo {
	typedef std::vector<AccountInfo> List;
	typedef std::vector<AccountInfo>::iterator iterator;
	typedef std::vector<AccountInfo>::const_iterator const_iterator;

	bool active;
	uint32_t authid;
	Glib::ustring authkey;
	CharacterInfo::List characters;

	bool load(const boost::property_tree::ptree &pt);
	bool save(boost::property_tree::ptree &pt) const;
};

class Settings {
public:
	Settings(int argc, char **argv);
	Settings();
	virtual ~Settings();

	bool process_command_line();

	/*! Provides an Exception-free access to the Settings value 'path' as Glib::ustring
	 *
	 * @param the path to the value
	 * @param default value if there is no entry 'path'
	 * @return either the value of 'path' or the default value
	 */
	Glib::ustring get_string(const Glib::ustring path, const Glib::ustring default_value = "");

	/*! Provides an Exception-free access to the Settings value 'path' as boolean
	 *
	 * @param the path to the value
	 * @param default value if there is no entry 'path'
	 * @return either the value of 'path' or the default value
	 */
	bool get_boolean(const Glib::ustring path, const bool default_value = false);

	/*! Provides an Exception-free access to the Settings value 'path' as integer
	 *
	 * @param the path to the value
	 * @param default value if there is no entry 'path'
	 * @return either the value of 'path' or the default value
	 */
	int32_t get_integer(const Glib::ustring path, const int32_t default_value = 0);

	//! returns the plugin list
	inline PluginInfo::List& get_plugin_list() {
		return m_plugin_list;
	}

	//! returns the account info list
	inline AccountInfo::List& get_account_list() {
		return m_account_list;
	}

	//! returns the config directory
	inline Glib::ustring get_config_directory() const {
		return c_config_directory;
	}

	//TODO: use #if HAVE_LOG4CXX to remove this
	//! returns the directory where the log file should be stored
	inline Glib::ustring get_log_directory(const Glib::ustring filename) const {
		if(filename.empty()) {
			return Glib::build_filename(c_config_directory, "logs");
		} else {
			return Glib::build_filename(c_config_directory, "logs", filename);
		}
	}

	//! returns the working directory for libeapi
	inline Glib::ustring get_eapi_directory() const {
		return Glib::build_filename(c_config_directory, "eveapi");
	}

	//! returns the data directory as Glib::ustring
	inline Glib::ustring get_data_directory() const {
		return c_data_directory;
	}

	//! returns the directory with plugins as Glib::ustring
	inline Glib::ustring get_plugin_directory() const {
		return Glib::build_filename(c_data_directory, "plugins");
	}

	//! returns the directory containing the glade files
	inline Glib::ustring get_glade_directory(const Glib::ustring filename = "") const {
		if(filename.empty()) {
			return Glib::build_filename(c_data_directory, "ui");
		} else {
			return Glib::build_filename(c_data_directory, "ui", filename);
		}
	}

	//! returns the directory containing the image files
	inline Glib::ustring get_media_directory(const Glib::ustring filename = "") const {
		if(filename.empty()) {
			return Glib::build_filename(c_data_directory, "media");
		} else {
			return Glib::build_filename(c_data_directory, "media", filename);
		}
	}

protected:
	void load(const Glib::ustring filename);
	void save(const Glib::ustring filename);

	template <class T>
	T get(const Glib::ustring path, const T default_value);

protected:
	static boost::property_tree::ptree pt;

	static const char q_config_subdir[];
	static Glib::ustring c_data_directory;
	static Glib::ustring c_config_directory;

private:
	bool print_version;
	bool print_help;

	PluginInfo::List m_plugin_list;
	AccountInfo::List m_account_list;
};

class UstringTranslator {
public:
	typedef std::string internal_type;
	typedef Glib::ustring external_type;

	boost::optional<external_type> get_value(internal_type const &v);
	boost::optional<internal_type> put_value(external_type const &v);
};

} /* namespace Quafe */
#endif /* SETTINGS_H_ */
