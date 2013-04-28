/*
 * pluginmanager.h
 *
 *  Created on: 02.03.2012
 *      Author: cn
 */

#ifndef PLUGINMANAGER_H_
#define PLUGINMANAGER_H_

#include <quafe-config.h>

#include "singleton.h"
#include "plugins/plugininterface.h"

#include <boost/noncopyable.hpp>

namespace Quafe {

class PluginManager : boost::noncopyable {
private:
	struct PluginHandle;
	typedef std::vector<PluginHandle*> PluginHandleList;

public:
	PluginManager();
	virtual ~PluginManager();

	void open_all();
	bool open(const Glib::ustring &id);

	void close_all();
	bool close(const Glib::ustring &id);


	bool find(const Glib::ustring id, PluginInterface *&plugin);

#if !QUAFE_BUILD_RELEASE
//	void hard_reset(PluginInfo &info);
#endif

protected:
	bool open(PluginHandle *info);
	bool close(PluginHandle *info);
	bool find(const Glib::ustring id, PluginHandle *&hdl);
	void read_plugin_dir();
private:
	PluginHandleList m_plugin_handles;

	Glib::ustring m_plugin_path;
};

}

#endif /* PLUGINMANAGER_H_ */
