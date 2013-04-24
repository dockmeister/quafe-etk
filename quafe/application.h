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


#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <quafe-config.h>

#include "singleton.h"

#include <gtkmm/main.h>
#include <gtkmm/object.h>



namespace Quafe {
// forward decl to minimize dependencies
class PluginInterface;
class ApplicationWindow;
class PluginManager;

/*! \brief Main application class.
 * 		Handles plugins, creates the window and finally runs the application.
 */
class Application : public Singleton<Application> {
	friend class Singleton<Application>;
public:
	void run(); //!< runs the application

	const ApplicationWindow * get_window();

protected:
	/*! \brief
	 *
	 *
	 */
	void quit();
	ApplicationWindow *app_window;

	bool on_plugin_enabled(const Glib::ustring &id);
	bool on_plugin_disabled(const Glib::ustring &id);
	bool on_plugin_activated(const Glib::ustring &id);
	bool on_plugin_deactivated(const Glib::ustring &id);

private:
	Application();
	virtual ~Application();

	bool running;
	PluginInterface *m_plugin_current; //!< current activated plugin
	PluginManager *mPluginManager;
};

}

#endif /* APPLICATION_H_ */
