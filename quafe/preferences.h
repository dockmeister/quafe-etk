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

#include "singleton.h"
#include "utils/exception.h"

#include <pugixml/pugixml.hpp>
#include <boost/any.hpp>
#include <boost/program_options.hpp>
#include <gtkmm/window.h>

namespace EAPI {
class KeyInfo;
}
namespace Quafe {
// forward decl to minimize dependencies
class PluginInterface;
class PreferenceDialog;

/*!\brief
 *
 */
struct AccountInfo {
	bool active;
	Glib::ustring authid;
	Glib::ustring authkey;
	std::pair<bool, Glib::ustring> characters[3];
};

typedef std::list<AccountInfo> AccountInfoList;

namespace po = boost::program_options;

typedef std::map<Glib::ustring, boost::any> PreferenceMap;

/*!\brief
 *
 */
class Preferences: public Singleton<Preferences> {
	friend class Singleton<Preferences> ;
public:
	static bool init(int argc, char **argv);

public:
	/*!\brief
	 * \param option the identifier to the option
	 * \return the value for option
	 */
	template<class ValueType>
	static const ValueType get(Glib::ustring option) {
		ValueType v;
		try {
			v = m_instance->vmap[option].as<ValueType>();
		} catch (...) {
			throw Exception("bad any cast: unknown option '" + option + "'");
		}
		return v;
	}

	/*!< creates the dialog. called from application after the main window is created */
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

	/*!\brief parses the accountlist node
	 * 		  We cannot parse the account list within parse_config_file since EAPI is not initialized then
	 */
	void parse_account_list();

	/*!\brief tries to save a config file
	 * \return true if the config file was successfully saved
	 */
	gboolean save_config_file();

protected:
	void on_api_add(const Glib::ustring &keyID, const Glib::ustring &vCode);
	void on_api_add_callback(EAPI::KeyInfo *key);

	void on_delete_api(const Glib::ustring &keyID);

	bool on_api_character_toggled(const Glib::ustring &keyID, const Glib::ustring &cID, bool active);


	/*< */
	void insert_api_to_treestore();

private:
	Preferences();
	virtual ~Preferences();

	template<typename T>
	struct translate {
		static void to(pugi::xml_node &node, const po::variable_value &value);
		static void to(pugi::xml_node &node, const T &value);
		static void to(pugi::xml_node &node, const T *&value);

		static void from(const pugi::xml_node &node, T &value);
	};

	Glib::ustring m_config_path;

	pugi::xml_document doc;
	po::variables_map vmap;
	po::options_description opt_cli, opt_general, opt_all;


	Glib::ustring default_config_path();
	Glib::ustring default_data_path();

	PreferenceDialog *m_dialog;
};
} /* namespace Quafe */
#endif /* PREFERENCES_H_ */
