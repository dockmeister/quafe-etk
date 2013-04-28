/*
 * pluginmanager.cc
 *
 *  Created on: 02.03.2012
 *      Author: cn
 */
#include <quafe-logging.h>
#include "pluginmanager.h"

#include "settings.h"

#include <dlfcn.h>
#include <boost/bind.hpp>
#include <glibmm/fileutils.h>
#include <glibmm/regex.h>
#include <glibmm/miscutils.h>

QUAFE_DECLARE_STATIC_LOGGER("Quafe");

namespace Quafe {

/*! \brief PluginInfo struct.
 * 		Plugin information are stored in a PluginContainer.
 * 		Use the function pointer 'create' and 'destroy' to handle the plugin.
 * 		'plugin_id' should be a unique string to identify the plugin.
 */
struct PluginManager::PluginHandle {
	PluginHandle(PluginInfo &info_)
			: info(info_), ptr(0), create(0), dlhandle(0), destroy(0) {
		autoload = info.autoload;
		id = info.id;
		file = info.file;
	}

	PluginInfo &info;
	Glib::ustring id;
	Glib::ustring file;

	Quafe::PluginInterface *ptr;
	Quafe::create_t *create; /*!< */
	void* dlhandle;
	Quafe::destroy_t *destroy;

	bool autoload;
};

PluginManager::PluginManager() {
	read_plugin_dir();
}

PluginManager::~PluginManager() {
	close_all();
}

void PluginManager::read_plugin_dir() {
	Settings settings;
	m_plugin_path = settings.get_plugin_directory();

	if (!Glib::file_test(m_plugin_path, Glib::FILE_TEST_IS_DIR)) {
		LOG_WARN("Plugin-directory not found: '%1'", m_plugin_path);
		return;
	}

	Glib::Dir plugin_dir(settings.get_plugin_directory());
	Glib::Dir::const_iterator it = plugin_dir.begin();

	PluginInfo::List pinfolist = settings.get_plugin_list();

	// by now Preferences filled  plugin_list with plugin-configuration
	for (; it != plugin_dir.end(); ++it) {
		if (!Glib::Regex::match_simple("^libquafe-[a-z]+.so$", *it))
			continue;

		PluginInfo::iterator p_it = std::find_if(pinfolist.begin(), pinfolist.end(), boost::bind(&PluginInfo::file, _1) == (*it));

		if (p_it == pinfolist.end()) {
			// Plugin is new, create a info struct and add it to the list
			static PluginInfo info;
			info.file = *it;
			info.autoload = true;
			info.found = true;

			pinfolist.push_back(info);
			PluginHandle *hdl = new PluginHandle(info);
			m_plugin_handles.push_back(hdl);
		} else {
			// plugin is already in list
			PluginInfo &info = *p_it;
			info = *p_it;
			info.found = true;
			PluginHandle *hdl = new PluginHandle(info);
			m_plugin_handles.push_back(hdl);
		}
	}

	// remove any plugin which has a previous settings entry, but is not available anymore
	std::remove_if(pinfolist.begin(), pinfolist.end(), boost::bind(&PluginInfo::found, _1) == false);

	LOG_INFO("Plugin path: %1 (%2 plugins discoverd)", m_plugin_path, m_plugin_handles.size());
}

void PluginManager::open_all() {
	for(PluginHandleList::const_iterator it = m_plugin_handles.begin(); it != m_plugin_handles.end(); it++) {
		if((*it)->autoload == true) {
			open((*it));
		}
	}
}

bool PluginManager::open(const Glib::ustring &id) {
	PluginHandle *hdl = 0;
	if(!find(id, hdl)) {
		LOG_WARN("Unable to load %1. Plugin not found", id);
		return false;
	}

	return open(hdl);
}

bool PluginManager::open(PluginHandle *hdl) {
	Glib::ustring plugin_file = Glib::build_filename(m_plugin_path, hdl->info.file);

	void* plugin = dlopen(plugin_file.c_str(), RTLD_LAZY);
	if (!plugin) {
		LOG_ERROR("Cannot load plugin: %1", dlerror());
		return false;
	}

	Quafe::create_t* create_plugin = (Quafe::create_t*) dlsym(plugin, "create");
	const char* dlsym_error = dlerror();
	if (dlsym_error) {
		LOG_ERROR("Cannot load symbol create: %1", dlsym_error);
		return false;
	}

	Quafe::destroy_t* destroy_plugin = (Quafe::destroy_t*) dlsym(plugin, "destroy");
	dlsym_error = dlerror();
	if (dlsym_error) {
		LOG_ERROR("Cannot load symbol destroy: %1", dlsym_error);
		return false;
	}

	Quafe::get_params_t* plugin_params = (Quafe::get_params_t*) dlsym(plugin, "get_params");
	dlsym_error = dlerror();
	if (dlsym_error) {
		LOG_ERROR("Cannot load symbol get_params: ", dlsym_error);
		return false;
	}

	hdl->dlhandle = plugin;
	hdl->create = create_plugin;
	hdl->destroy = destroy_plugin;
	hdl->info.id = plugin_params().id;
	hdl->info.title = plugin_params().title;
	hdl->info.icon = plugin_params().icon;

	if (hdl->ptr != 0) {
		LOG_WARN("failed to create plugin '%1': already created", hdl->id);
		return false;
	}

	if ((hdl->ptr = hdl->create()) == 0) {
		LOG_WARN("failed to create plugin '%1'", hdl->id);
		return false;
	}

	LOG_INFO("Plugin '%1' successfully opened.", hdl->id);
	return true;

	return true;
}

void PluginManager::close_all() {
	for(PluginHandleList::const_iterator it = m_plugin_handles.begin(); it != m_plugin_handles.end(); it++) {
		if((*it)->dlhandle != 0 &&  !(*it)->ptr) {
			open((*it));
		}
	}
}

bool PluginManager::close(const Glib::ustring &id) {
	PluginHandle *hdl = 0;
	if(!find(id, hdl)) {
		LOG_WARN("Unable to close %1. Plugin not found", id);
		return false;
	}

	return close(hdl);
}

bool PluginManager::close(PluginHandle *hdl) {
	if(hdl->destroy == 0) {
		LOG_WARN("Failed to close %1 destroy handle doenst exists", hdl->file);
		return false;
	}

	hdl->destroy(hdl->ptr);
	hdl->ptr = 0;

	if (dlclose(hdl->dlhandle) != 0) {
		LOG_WARN(dlerror());
		return false;
	}

	hdl->dlhandle = 0;
	hdl->create = 0;
	hdl->destroy = 0;

	LOG_INFO("Plugin %1 successfully closed.", hdl->info.id);
	return true;
}

bool PluginManager::find(const Glib::ustring id, PluginHandle *&hdl) {
	PluginHandleList::iterator it = std::find_if(m_plugin_handles.begin(), m_plugin_handles.end(), boost::bind(&PluginHandle::id, _1) == id);

	if (it != m_plugin_handles.end()) {
		hdl = (*it);
		return true;
	}
	return false;
}

bool PluginManager::find(const Glib::ustring id, PluginInterface *&plugin) {
	PluginHandle *hdl = 0;
	if (!find(id, hdl)) {
		return false;
	}

	if (hdl->ptr == 0) {
		LOG_INFO("Plugin '%1' found but its not created.", id);
		return false;
	}
	plugin = hdl->ptr;

	return true;
}

}
