/*
 * pluginmanager.h
 *
 *  Created on: 02.03.2012
 *      Author: cn
 */

#ifndef PLUGINMANAGER_H_
#define PLUGINMANAGER_H_

#include <quafe-etk.h>

#include "include/singleton.h"
#include "include/pluginbase.h"

#define DEBUG_ 1

namespace Quafe {

/*! \brief PluginInfo struct.
 * 		Plugin information are stored in a PluginContainer.
 * 		Use the function pointer 'create' and 'destroy' to handle the plugin.
 * 		'plugin_id' should be a unique string to identify the plugin.
 */
typedef struct {
	void* dlhandle;
	Quafe::create_t *create; /*!< */
	Quafe::destroy_t *destroy;
	Quafe::PluginBase *ptr;

	bool active;
	bool found;

	ustring file;
	ustring id;
	ustring title;
	ustring icon;

	bool validate() {
		if(id == "" || title == "" || icon == "")
			return false;

		return true;
	}
} PluginInfo;

typedef std::list<PluginInfo> PluginInfoList;

class PluginManager : public Singleton<PluginManager> {
	friend class Singleton<PluginManager>;
public:
	static void init(ustring &directory);
public:
	void open_all();
	bool open(PluginInfo &info);

	void close_all();
	bool close(PluginInfo &info);

	void create_all();
	bool create(PluginInfo &info);

	void destroy_all();
	void destroy(PluginInfo &info);

	bool find(const ustring id, PluginBase *&plugin);
	bool find(const ustring id, PluginInfo &info);

#ifdef DEBUG_
	void hard_reset(PluginInfo &info);
#endif

public:
	PluginInfoList & get_plugin_list();
public:
	virtual ~PluginManager();
protected:
	PluginManager();

	void read_plugin_dir(const ustring &directory);
private:
	PluginInfoList m_plugin_list;
	ustring m_plugin_path;

};

}

#endif /* PLUGINMANAGER_H_ */
