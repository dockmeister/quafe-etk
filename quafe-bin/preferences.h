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

#ifndef PREFERENCES_H_
#define PREFERENCES_H_

#include <quafe-etk.h>
#include <utility.h>

#include "include/pluginbase.h"
#include "ui/window_preferences.h"
#include <boost/any.hpp>
#include <boost/property_tree/ptree.hpp>

namespace Quafe {

/*! \brief PluginInfo struct.
 * 		Plugin information are stored in a PluginContainer.
 * 		Use the function pointer 'create' and 'destroy' to handle the plugin.
 * 		'plugin_id' should be a unique string to identify the plugin.
 */
struct PluginInfo {
	Quafe::create_t *create; /*!< */
	Quafe::destroy_t *destroy;
	Quafe::PluginBase *ptr;

	gboolean active;
	ustring plugin_id;
};

/*!\brief
 *
 */
struct AccountInfo {
	gboolean active;
	ustring authid;
	ustring authkey;
	std::pair<gboolean, ustring> characters[3];
};

typedef std::list<AccountInfo> AccountInfoList;
typedef std::list<PluginInfo> PluginInfoList;
typedef std::map<ustring, boost::any> PreferenceMap;

/*!\brief
 *
 */
class Preferences : public Singleton<Preferences> {
	friend class Singleton<Preferences>;
public:
	/*!\brief
	 * \param option the identifier to the option
	 * \return the value for option
	 */
	template <class ValueType>
	static const ValueType& get(ustring option) {
		return boost::any_cast<ValueType>(instance()->m_settings[option]);
	}

	/*!\brief opens the settings window */
	void show_settings();

	/*!\brief CLI only. Parses any cli-arguments / prints help or version
	 *
	 * \param argc
	 * \param argv
	 * \return true if --help or --version is an argument.
	 */
	gboolean parse_command_line(int argc, char **argv);

	/*!\brief tries to load a config file
	 * \return true if a config file was loaded
	 */
	gboolean parse_config_file();

	/*!\brief tries to save a config file
	 * \return true if the config file was successfully saved
	 */
	gboolean save_config_file();

	/*!\brief
	 *
	 */
	void discover_plugins();
private:
	Preferences();
	virtual ~Preferences();
	
	ustring discover_config_path();
	ustring discover_plugin_path();

	PreferenceMap m_settings;


	WindowPreferences sett_window;


};

} /* namespace Quafe */
#endif /* PREFERENCES_H_ */
