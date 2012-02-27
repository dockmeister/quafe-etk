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


#ifndef PLUGINBASE_H_
#define PLUGINBASE_H_

#include <quafe-etk.h>

#include <gtkmm/widget.h>
/*
 *
 */
namespace Quafe {

struct PluginConstructParams {
	ustring plugin_id;
	ustring plugin_title;
	ustring plugin_description;
	ustring plugin_icon;
};
/*!\brief
 *
 */
class PluginBase {
public:
	PluginBase() : m_plugin_widget(0) {

	};
	virtual ~PluginBase() {
		if(m_plugin_widget)
			delete m_plugin_widget;
	};

	virtual Gtk::Widget & show() const {
		return *m_plugin_widget;
	}
	virtual gboolean close() const = 0;
	virtual ustring plugin_id() const = 0;
	virtual ustring plugin_title() const = 0;
	virtual ustring plugin_description() const = 0;
	virtual ustring plugin_icon_path() const = 0;
	Gtk::Widget *m_plugin_widget;
};
// the types of the class factories
typedef PluginBase* create_t();
typedef void destroy_t(PluginBase*);

}

#endif /* PLUGINBASE_H_ */
