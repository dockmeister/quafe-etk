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


#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <quafe-etk.h>
#include <utility.h>

#include "ui/window.h"
#include "include/pluginbase.h"
#include <gtkmm/main.h>
#include <gtkmm/object.h>

namespace Quafe {

/*! \brief PluginContainer struct.
 * 		Plugin information are stored in a PluginContainer.
 * 		Use the function pointer 'create' and 'destroy' to handle the plugin.
 * 		'plugin_id' should be a unique string to identify the plugin.
 */
struct PluginContainer {
	Quafe::create_t *create; /*!< */
	Quafe::destroy_t *destroy;

	Quafe::PluginBase *ptr;
	ustring plugin_id;
};

typedef std::list<PluginContainer> PluginList;

/*! \brief Main application class.
 * 		Handles plugins, creates the window and finally runs the application.
 */
class Application : public Singleton<Application> {
	friend class Singleton<Application>;
public:
	void run(); //!< runs the application

	/*!\brief
	 *
	 * \param plugin_list
	 */
	void load_plugins(const PluginList & plugin_list);

	/*!\brief
	 *
	 * \param plugin_id
	 * \return true, if the plugin switch was successful
	 */
	gboolean toggle_plugin(ustring plugin_id);
protected:
	/**
	 *
	 */
	void quit();
	Window app_window;

private:
	Application();
	virtual ~Application();

	PluginList m_plugin_list; //!< list of plugins found
	PluginBase *m_plugin_current; //!< current activated plugin
};

}

#endif /* APPLICATION_H_ */
