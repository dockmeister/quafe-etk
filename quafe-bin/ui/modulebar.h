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


#ifndef MODULEBAR_H
#define	MODULEBAR_H

#include <quafe-etk.h>
#include <utility.h>

#include <boost/function.hpp>
#include <gdkmm/color.h>
#include <gtkmm/frame.h>
#include <gtkmm/box.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/stylecontext.h>

namespace Quafe {

typedef std::list<Gtk::ToggleButton *> ButtonList;

/** Widget ModuleBar
 *
 * @param mod
 */
class ModuleBar: public Gtk::Frame {
public:
	/**
	 *
	 * @param cobject
	 * @param ref_builder
	 */
	ModuleBar();

	virtual ~ModuleBar();
	/** Adds a module to the list, creates a button and a label.
	 *
	 * @param mod Glib::RefPtr to a module
	 */
	virtual void add_plugin_button(ustring plugin_id, ustring image_path, ustring str_label);

	/**
	 *
	 */
	virtual void change_style();

	boost::function<gboolean(ustring)> action_plugin_requested;

protected:
	// default con- / destructors
	void pre_plugin_button_clicked(Gtk::ToggleButton *button_clicked, ustring plugin_id);

private:
	RefPtr<Gtk::CssProvider> m_refCssProvider;
	Gtk::ToggleButton *button_active;

	/** methods to darken background color
	 *
	 * @see planner v.14
	 */
	static void hls_to_rgb(gdouble &h, gdouble &l, gdouble &s);
	static void rgb_to_hls(gdouble &r, gdouble &g, gdouble &b);
	static void style_shade(Gdk::RGBA &a, Gdk::RGBA &b, gdouble k);
	static void darken_color(Gdk::RGBA &color, gint darken_count);

	ButtonList button_list;

	Gdk::RGBA c_normal, c_prelight;
	Gtk::EventBox *m_eventbox;
	Gtk::VBox *m_vbox;

	gboolean signal_handler_running;
};

}
#endif	/* MODULEBAR_H */

