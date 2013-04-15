/*
 * pluginmanager.h
 *
 *  Created on: 02.03.2012
 *      Author: cn
 */

#ifndef PLUGINMANAGER_H_
#define PLUGINMANAGER_H_

#include "singleton.h"
#include "plugins/plugininterface.h"

#define DEBUG_ 1

namespace Quafe {

/*! \brief PluginInfo struct.
 * 		Plugin information are stored in a PluginContainer.
 * 		Use the function pointer 'create' and 'destroy' to handle the plugin.
 * 		'plugin_id' should be a unique string to identify the plugin.
 */
struct PluginInfo {
	void* dlhandle;
	Quafe::create_t *create; /*!< */
	Quafe::destroy_t *destroy;
	Quafe::PluginInterface *ptr;

	bool active;
	bool found;

	Glib::ustring  file;
	Glib::ustring  id;
	Glib::ustring  title;
	Glib::ustring  icon;

	bool validate() {
		if(id == "" || title == "" || icon == "")
			return false;

		return true;
	}

	PluginInfo() {
		ptr = 0;
		create = 0;
		dlhandle = 0;
		destroy = 0;
		create = 0;
		found = false;
		active = false;
	}
};

typedef std::list<PluginInfo> PluginInfoList;

class PluginManager : public Singleton<PluginManager> {
	friend class Singleton<PluginManager>;
public:
	static void init(Glib::ustring &directory);
public:
	void open_all();
	bool open(PluginInfo &info);

	void close_all();
	bool close(PluginInfo &info);

	void create_all();
	bool create(PluginInfo &info);

	void destroy_all();
	void destroy(PluginInfo &info);

	bool find(const Glib::ustring id, PluginInterface *&plugin);
	bool find(const Glib::ustring id, PluginInfo &info);

#ifdef DEBUG_
	void hard_reset(PluginInfo &info);
#endif

public:
	PluginInfoList & get_plugin_list();
public:
	virtual ~PluginManager();
protected:
	PluginManager();

	void read_plugin_dir(const Glib::ustring &directory);
private:
	PluginInfoList m_plugin_list;
Glib::ustring  m_plugin_path;

};

}

#endif /* PLUGINMANAGER_H_ */
