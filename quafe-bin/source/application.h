/*
 * application.h
 *
 *  Created on: 08.02.2012
 *      Author: cn
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <quafe-etk.h>

#include "ui/window.h"
#include "plugin/pluginbase.h"
#include <gtkmm/main.h>
/*
 *
 */
namespace Quafe {

struct PluginContainer {
	Quafe::create_t *create;
	Quafe::destroy_t *destroy;

	Quafe::PluginBase *ptr;
	ustring plugin_id;
};

typedef std::list<PluginContainer> PluginList;

class Application {
public:
	Application();
	~Application();
	void run(Gtk::Main &kit);
	void quit();
	void plugins_create();
	void plugins_destroy();
	PluginList& plugins_getlist() {return m_plugin_list;};

	void toggle_plugin(ustring plugin_id);
protected:
	Window app_window;

private:
	PluginList m_plugin_list;
	PluginBase *m_plugin_current;
};

}

#endif /* APPLICATION_H_ */
