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

#ifndef PREFERENCES_H_
#define PREFERENCES_H_

#include <quafe-etk.h>

#include "include/pluginbase.h"
#include "include/singleton.h"
#include <pugixml/pugixml.hpp>
#include <boost/any.hpp>
#include <boost/program_options.hpp>

namespace Quafe {

// forward decl to minimize dependencies
class PluginDecl;
class PluginBase;
class PreferenceDialog;

namespace po = boost::program_options;


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
typedef std::map<ustring, boost::any> PreferenceMap;

/*!\brief
 *
 */
class Preferences : public Singleton<Preferences> {
	friend class Singleton<Preferences>;
public:
	static bool init(int argc, char **argv);

public:
	/*!\brief
	 * \param option the identifier to the option
	 * \return the value for option
	 */
	template <class ValueType>
	static const ValueType& get(ustring option) {
		//ValueType &s = boost::any_cast<ValueType>(&(instance()->m_settings[option]));
		return instance()->vmap[option].as<ValueType>();
	}

	/*< creates the dialog. called from application after the main window is created */
		void create_dialog(Gtk::Window &window);

	/*!\brief opens the settings window */
	void show_settings();

	/*! \brief CLI only. Parses any cli-arguments / prints help or version
	 *
	 *  \param argc
	 *  \param argv
	 *  \return true if --help or --version is an argument.
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

protected:



	/*< applies changes made in the dialog to AccountInfoList */
	void apply_api_changes(const ustring &auth_id, const ustring &auth_key, API_CHANGE chg);

	/*< */
	void insert_api_to_treestore();

private:
	Preferences();
	virtual ~Preferences();
	
	template <typename T>
	struct translate {
		static void to(pugi::xml_node &node, const po::variable_value &value);
		static void to(pugi::xml_node &node, const T &value);

		static void from(const pugi::xml_node &node, T &value);
	};

	ustring m_config_path;

	AccountInfoList m_account_list;
	po::variables_map vmap;
	po::options_description opt_cli, opt_general, opt_all;

	ustring default_config_path();
	ustring default_data_path();



	PreferenceDialog *m_dialog;
};
} /* namespace Quafe */
#endif /* PREFERENCES_H_ */
