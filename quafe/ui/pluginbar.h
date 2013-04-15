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

#ifndef PLUGINBAR_H
#define	PLUGINBAR_H

#include "pluginbutton.h"

#include <map>
#include <boost/signals2.hpp>
#include <gtkmm/builder.h>
#include <gdkmm/color.h>
#include <gtkmm/frame.h>
#include <gtkmm/box.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/label.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/stylecontext.h>

namespace Quafe {


typedef std::map<Glib::ustring, PluginButton*> ButtonList;

/** Widget ModuleBar
 *
 * @param mod
 */
class Pluginbar: public Gtk::Frame {
public:
	typedef boost::signals2::signal<bool (const Glib::ustring &id)> signal_plugin_t;
public:
	/**
	 *
	 * @param cobject
	 * @param ref_builder
	 */
	Pluginbar(BaseObjectType *cobject, Glib::RefPtr<Gtk::Builder> refBuilder);

	virtual ~Pluginbar();
	/** Adds a module to the list, creates a button and a label.
	 *
	 * @param mod Glib::RefPtr to a module
	 */
	virtual void add_plugin_button(Glib::ustring plugin_id, Glib::ustring image_path, Glib::ustring str_label);

	/**
	 *
	 */
	virtual void change_style(Gtk::Widget *);

	void show_plugin_button(const Glib::ustring &id);
	void hide_plugin_button(const Glib::ustring &id);

	inline boost::signals2::connection signal_plugin_activated(const signal_plugin_t::slot_type &slot) {
		return m_signal_plugin_activated.connect(slot);
	}

	inline boost::signals2::connection signal_plugin_deactivated(const signal_plugin_t::slot_type &slot) {
		return m_signal_plugin_deactivated.connect(slot);
	}
protected:
	void on_pluginbutton_toggled(PluginButton *button_clicked);

private:
	Glib::RefPtr<Gtk::Builder> m_refBuilder;
	Glib::RefPtr<Gtk::CssProvider> m_refCssProvider;
	Glib::ustring css_data;

	ButtonList button_list;
	PluginButton *m_currpb;
	signal_plugin_t m_signal_plugin_activated, m_signal_plugin_deactivated;

	Gdk::RGBA c_normal, c_prelight;
	Gtk::EventBox *m_eventbox;
	Gtk::VBox *m_vbox;

	bool signal_handler_running;

	/** methods to darken background color
	 *
	 * @see planner v.14
	 */
	static void hls_to_rgb(gdouble &h, gdouble &l, gdouble &s);
	static void rgb_to_hls(gdouble &r, gdouble &g, gdouble &b);
	static void style_shade(Gdk::RGBA &a, Gdk::RGBA &b, gdouble k);
	static void darken_color(Gdk::RGBA &color, gint darken_count);


};

}
#endif	/* PLUGINBAR_H */

