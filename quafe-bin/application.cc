/*
 * application.cpp
 *
 *  Created on: 08.02.2012
 *      Author: cn
 */

#include "application.h"
#include "log.h"

#include <boost/bind.hpp>
#include <gtkmm/main.h>
#include <gtkmm/togglebutton.h>

namespace Quafe {

Application::Application() :
	m_plugin_current(0) {
	// bind actions before creating the window content
	app_window.on_action_file_quit = boost::bind(&Application::quit, this);
	app_window.m_ptrModulebar->action_plugin_requested = boost::bind(&Application::toggle_plugin, this, _1);

	app_window.create_window();
}

Application::~Application() {
	plugins_destroy();
}

void Application::run(Gtk::Main &kit) {
	app_window.show_all();
	kit.run(app_window);

}

void Application::quit() {
	LOG(L_NOTICE) << "quafe-etk shutting down..." << m_plugin_list.size();
	app_window.hide();
}

// ------------------------------------------------------------------------------------------
// Plugin methods0x798150
gboolean Application::toggle_plugin(ustring plugin_id) {
	PluginBase *req_plg = 0;
	PluginList::iterator it = m_plugin_list.begin();
	for (; it != m_plugin_list.end(); ++it) {
		if (plugin_id.compare((*it).ptr->plugin_id()) == 0) {
			req_plg = (*it).ptr;
			break;
		}
	}

	if (req_plg == 0) {
		LOG(L_ERROR) << "Unknown plugin '" << plugin_id << "' requested";
		return false;
	}

	if (req_plg == m_plugin_current) {
		LOG(L_DEBUG) << "Plugin '" << plugin_id << "' already loaded.";
		return false;
	}

	if (m_plugin_current == 0 || (m_plugin_current && m_plugin_current->close())) {
		m_plugin_current = req_plg;
		app_window.m_refContentFrame.remove();
		m_plugin_current->show(app_window.m_refContentFrame);
		app_window.m_refContentFrame.show_all_children(true);

		app_window.set_focus(app_window.m_refContentFrame);

		return true;
	}

	return false;
}

void Application::plugins_create() {
	PluginList::iterator it = m_plugin_list.begin();
	for (; it != m_plugin_list.end(); it++) {
		// create plugin and init
		(*it).ptr = (*it).create();
		(*it).plugin_id = (*it).ptr->plugin_id();

		app_window.m_ptrModulebar->add_plugin_button((*it).ptr->plugin_id(), (*it).ptr->plugin_icon_path(), (*it).ptr->plugin_title());

		LOG(L_NOTICE) << "Plugin '" << (*it).ptr->plugin_id() << "' loaded.";
	}
}

void Application::plugins_destroy() {
	m_plugin_current = 0;
	PluginList::iterator it = m_plugin_list.begin();
	for (; it != m_plugin_list.end(); it++) {
		ustring plugin_info = (*it).ptr->plugin_id();

		(*it).destroy((*it).ptr);
		(*it).ptr = 0;

		LOG(L_NOTICE) << "Plugin '" << plugin_info << "' unloaded.";
	}

	m_plugin_list.clear();
}
}
