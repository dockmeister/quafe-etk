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


#ifndef PLUGININTERFACE_H_
#define PLUGININTERFACE_H_

#include <gtkmm/widget.h>

namespace Quafe {

/*!\brief PluginParams needed by the pluginmanager
 *
 */
typedef struct {
	Glib::ustring id; //!< the _unique_ plugin id
	Glib::ustring title; //!< the title of the plugin
	Glib::ustring description; //!< a meaningful plugin description
	Glib::ustring icon; //!< relative path+filename (data-directory)
	guint32 wanted_apis;
} PluginParams;

/*!\brief
 *
 */
class PluginInterface {
public:
	PluginInterface(const PluginParams &params_) :
		params(params_), m_plugin_widget(0) {
	};
	virtual ~PluginInterface() {
		if(m_plugin_widget)
			delete m_plugin_widget;
	};

	virtual Gtk::Widget & show() = 0;
	virtual void hide() = 0;
	virtual bool close() = 0;

	virtual inline const Glib::ustring& get_id() const {
		return params.id;
	};
	virtual inline const Glib::ustring& get_title() const {
		return params.title;
	};
	virtual inline const Glib::ustring& get_description() const {
		return params.description;
	};
	virtual inline const Glib::ustring& get_icon_path() const {
		return params.icon;
	};

protected:
	virtual void build_plugin_widget(const Glib::ustring glade_filename, const Glib::ustring glade_object) {

	}

private:
	PluginParams params;
	Gtk::Widget *m_plugin_widget;

};

// the types of the class factories
typedef const PluginParams & get_params_t();
typedef PluginInterface* create_t();
typedef void destroy_t(PluginInterface*);

}

#endif /* PLUGININTERFACE_H_ */
