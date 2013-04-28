/**
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

#ifndef APPLICATIONWINDOW_H_
#define APPLICATIONWINDOW_H_

#include <quafe-config.h>

#include "pluginbar.h"

#include <boost/signals2.hpp>
#include <gtkmm/builder.h>
#include <gtkmm/window.h>
#include <gtkmm/stock.h>
#include <gtkmm/menu.h>
#include <gtkmm/menubar.h>
#include <gtkmm/checkmenuitem.h>
#include <gtkmm/scrolledwindow.h>

namespace Quafe {

class Pluginbar;

/*!\brief
 *
 */
class ApplicationWindow: public Gtk::Window {
public:
	typedef boost::signals2::signal<bool(const Glib::ustring &)> signal_plugin_toggle_t;

public:
	//!< Use Gtk::Builder to get this window derived
	ApplicationWindow(BaseObjectType *cobject, Glib::RefPtr<Gtk::Builder> refBuilder);

	~ApplicationWindow();

	/*!\brief Notifies the GUI of a new plugin. It creates a ToggleButton for the Pluginbar and
	 * a MenuItem entry to en/disable the plugin
	 *
	 * @param plugin_id	the unique plugin id
	 * @param plugin_title the title of the plugin is used for the ToggleButton label
	 * @param plugin_icon the icon displayed inside of the ToggleButton
	 * @param active whether the plugin is loaded or not
	 */
	void add_plugin(Glib::ustring plugin_id, Glib::ustring plugin_title, Glib::ustring plugin_icon, bool active);

	/*!\brief
	 *
	 * @param widget
	 */
	void show_plugin_widget(Gtk::Widget &widget);

	//!< returns whether the window is maximized
	inline bool is_maximized() const {
		return maximized;
	}

	inline boost::signals2::connection signal_plugin_enabled(const signal_plugin_toggle_t::slot_type &slot) {
		return m_signal_plugin_enabled.connect(slot);
	}

	inline boost::signals2::connection signal_plugin_disabled(const signal_plugin_toggle_t::slot_type &slot) {
		return m_signal_plugin_disabled.connect(slot);
	}

	inline boost::signals2::connection signal_plugin_activated(const signal_plugin_toggle_t::slot_type &slot) {
		return m_pluginbar->signal_plugin_activated(slot);
	}

	inline boost::signals2::connection signal_plugin_deactivated(const signal_plugin_toggle_t::slot_type &slot) {
		return m_pluginbar->signal_plugin_deactivated(slot);
	}

protected:


	bool on_window_state_event(GdkEventWindowState *event);

	void on_menuitem_plugin(Gtk::CheckMenuItem *item, Glib::ustring plugin_id);
private:
	Glib::RefPtr<Gtk::Builder> m_refBuilder;

	Pluginbar *m_pluginbar;
	Gtk::ScrolledWindow *m_pluginframe;
	Gtk::MenuItem *m_pluginMenu; //< used to add menuitems 'plugins'

	signal_plugin_toggle_t m_signal_plugin_enabled, m_signal_plugin_disabled;

	bool maximized;

};

}

#endif /* APPLICATIONWINDOW_H_ */
