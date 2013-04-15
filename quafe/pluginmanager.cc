/*
 * pluginmanager.cc
 *
 *  Created on: 02.03.2012
 *      Author: cn
 */
#include <quafe-config.h>

#include "pluginmanager.h"


#include <dlfcn.h>
#include <boost/bind.hpp>
#include <glibmm/fileutils.h>
#include <glibmm/regex.h>
#include <glibmm/miscutils.h>

QUAFE_DECLARE_STATIC_LOGGER("EAPI");

namespace Quafe {

void PluginManager::init(Glib::ustring &directory) {
	PluginManager *pm = PluginManager::instance();
	pm->read_plugin_dir(directory);
	pm->open_all();
	pm->create_all();
}

void PluginManager::read_plugin_dir(const Glib::ustring &directory) {
	m_plugin_path = directory;

	if (!Glib::file_test(m_plugin_path, Glib::FILE_TEST_IS_DIR)) {
		// TODO throw exception
		LOG_WARN("Plugin-directory not found: '" << m_plugin_path << "'");
		return;
	}

	Glib::Dir plugin_dir(m_plugin_path);
	Glib::Dir::const_iterator it = plugin_dir.begin();

	// by now Preferences filled  plugin_list with plugin-configuration
	for (; it != plugin_dir.end(); ++it) {
		if (!Glib::Regex::match_simple("^libquafe-[a-z]+.so$", *it))
			continue;

		PluginInfo info;
		PluginInfoList::iterator p_it = std::find_if(m_plugin_list.begin(), m_plugin_list.end(), boost::bind(&PluginInfo::file, _1) == *it);

		if (p_it == m_plugin_list.end()) {
			// Plugin is new, create a info struct and add it to the list
			info.file = *it;
			info.active = true;
			info.found = true;

			m_plugin_list.push_back(info);
		} else {
			// plugin is already in list
			info = *p_it;
			info.found = true;
		}
	}

	// TODO remove plugins which are not available but have a configuration

	LOG_INFO("Plugin path: " << m_plugin_path << " (" << m_plugin_list.size() << " plugins discovered)");
}

void PluginManager::open_all() {
	std::for_each(m_plugin_list.begin(), m_plugin_list.end(), boost::bind(&PluginManager::open, this, _1));
	// TODO remove if open failed std::erase_if
}

bool PluginManager::open(PluginInfo &info) {
	Glib::ustring plugin_file = Glib::build_filename(m_plugin_path, info.file);

	void* plugin = dlopen(plugin_file.c_str(), RTLD_LAZY);
	if (!plugin) {
		LOG_ERROR("Cannot load plugin " << dlerror());
		return false;
	}

	Quafe::create_t* create_plugin = (Quafe::create_t*) dlsym(plugin, "create");
	const char* dlsym_error = dlerror();
	if (dlsym_error) {
		LOG_ERROR("Cannot load symbol create: " << dlsym_error);
		return false;
	}

	Quafe::destroy_t* destroy_plugin = (Quafe::destroy_t*) dlsym(plugin, "destroy");
	dlsym_error = dlerror();
	if (dlsym_error) {
		LOG_ERROR("Cannot load symbol destroy: " << dlsym_error);
		return false;
	}

	Quafe::get_params_t* plugin_params = (Quafe::get_params_t*) dlsym(plugin, "get_params");
	dlsym_error = dlerror();
	if (dlsym_error) {
		LOG_ERROR("Cannot load symbol get_params: " << dlsym_error);
		return false;
	}

	info.dlhandle = plugin;
	info.ptr = 0;
	info.create = create_plugin;
	info.destroy = destroy_plugin;
	info.id = plugin_params().id;
	info.title = plugin_params().title;
	info.icon = plugin_params().icon;

	return true;
}

void PluginManager::close_all() {
	std::for_each(m_plugin_list.begin(), m_plugin_list.end(), boost::bind(&PluginManager::close, this, _1));
}

bool PluginManager::close(PluginInfo &info) {
	if (info.dlhandle == 0 || info.ptr) {
		LOG_WARN("Failed to close '" << info.file << "'");
		return false;
	}

	if (dlclose(info.dlhandle)) {
		// dlclose != 0 : error occured
		LOG_WARN(dlerror());
		return false;
	}

	info.dlhandle = 0;
	return true;
}

void PluginManager::create_all() {
	PluginInfoList::iterator it = m_plugin_list.begin();
	for (; it != m_plugin_list.end(); ++it) {
		if ((*it).active == true) {
			create(*it);
		}
		// TODO remove if create failed std::erase_if
	}
}

bool PluginManager::create(PluginInfo &info) {
	if (info.ptr) {
		LOG_WARN("failed to create plugin '" << info.id << "': already created");
		return false;
	}
	info.ptr = info.create();
	if (info.ptr == 0) {
		LOG_WARN("failed to create plugin '" << info.id << "'");
		return false;
	}

	LOG_INFO("Plugin '" << info.id << "' created.");
	return true;
}

void PluginManager::destroy_all() {
	std::for_each(m_plugin_list.begin(), m_plugin_list.end(), boost::bind(&PluginManager::destroy, this, _1));
}
void PluginManager::destroy(PluginInfo &info) {
	if (info.ptr == 0)
		return;

	info.destroy(info.ptr);
	info.ptr = 0;

	LOG_INFO("Plugin '" << info.id << "' destroyed.");
}

bool PluginManager::find(const Glib::ustring id, PluginInfo &info) {
	PluginInfoList::iterator it = std::find_if(m_plugin_list.begin(), m_plugin_list.end(), boost::bind(&PluginInfo::id, _1) == id);

	if (it != m_plugin_list.end()) {
		info = (*it);
		return true;
	}
	return false;
}

bool PluginManager::find(const Glib::ustring id, PluginInterface *&plugin) {
	PluginInfo info;
	if (!find(id, info)) {
		return false;
	}

	if (!info.ptr) {
		LOG_INFO("Plugin '" << id << "' found but its not created.");

		return false;
	}
	plugin = info.ptr;

	return true;
}

PluginInfoList & PluginManager::get_plugin_list() {
	return m_plugin_list;
}

PluginManager::PluginManager() {
}

PluginManager::~PluginManager() {
	destroy_all();
	close_all();
}

}
