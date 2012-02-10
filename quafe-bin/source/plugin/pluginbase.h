/*
 * pluginbase.h
 *
 *  Created on: 07.02.2012
 *      Author: cn
 */

#ifndef PLUGINBASE_H_
#define PLUGINBASE_H_

#include <quafe-etk.h>

#include <gtkmm/widget.h>
/*
 *
 */
namespace Quafe {

class PluginBase {
public:
	PluginBase() {};
	virtual ~PluginBase() {};

	virtual void show(Gtk::Widget &) = 0;
	virtual gboolean close() const = 0;
	virtual ustring plugin_id() const = 0;
	virtual ustring plugin_title() const = 0;
	virtual ustring plugin_description() const = 0;
	virtual ustring plugin_icon_path() const = 0;
};
// the types of the class factories
typedef PluginBase* create_t();
typedef void destroy_t(PluginBase*);

}

#endif /* PLUGINBASE_H_ */
