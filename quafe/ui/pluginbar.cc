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

#include <quafe-logging.h>

#include "pluginbar.h"
#include "../settings.h"

#include <boost/bind.hpp>
#include <giomm/file.h>
#include <giomm/fileinputstream.h>
#include <glibmm/miscutils.h>
#include <glibmm/fileutils.h>
#include <gtkmm/label.h>
#include <gtkmm/button.h>
#include <gtkmm/image.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/togglebutton.h>

QUAFE_DECLARE_STATIC_LOGGER("EAPI");

namespace Quafe {

// ----------------------------------------------------------------------------------------
Pluginbar::Pluginbar(BaseObjectType *cobject, Glib::RefPtr<Gtk::Builder> refBuilder)
		: Gtk::Frame(cobject), m_refBuilder(refBuilder), m_currpb(0), signal_handler_running(false) {
	// remove all childs from Gtk::Frame
	remove();

	// creating the Eventbox
	m_eventbox = Gtk::manage(new Gtk::EventBox);
	m_eventbox->signal_realize().connect(boost::bind(&Pluginbar::change_style, this, m_eventbox));

	m_vbox = Gtk::manage(new Gtk::VBox);
	m_vbox->set_border_width(6);

	m_eventbox->add(*m_vbox);
	add(*m_eventbox);

	set_shadow_type(Gtk::SHADOW_ETCHED_OUT);

	//[ load pluginbutton / eventbox styles
	Settings settings;
	Glib::ustring css_file = settings.get_glade_directory("pluginbutton.css");
	if(Glib::file_test(css_file, Glib::FILE_TEST_EXISTS)) {
		Glib::RefPtr<Gio::FileInputStream> stream = Gio::File::create_for_path(css_file)->read();
		goffset size = stream->query_info()->get_size();

		gchar buf[size];
		memset(buf, 0, size);
		gsize bytes_read = 0;
		stream->read_all(buf, size, bytes_read);

		css_data = Glib::ustring(buf, bytes_read);
	}
	//]
}

Pluginbar::~Pluginbar() {

}

void Pluginbar::on_pluginbutton_toggled(PluginButton *pb) {
	if (signal_handler_running)
		return;

	signal_handler_running = true;

	if (pb == m_currpb) {
		pb->set_active(true);
		signal_handler_running = false;
		return;
	}

	// try to close current plugin
	if (m_currpb && !m_signal_plugin_deactivated(m_currpb->get_plugin_id())) {
		LOG_WARN("unable to deactivate plugin");
		signal_handler_running = false;
		return;
	}

	// try to load requested plugin
	if (!m_signal_plugin_activated(pb->get_plugin_id())) {
		pb->set_active(false);
		signal_handler_running = false;
		return;
	}

	if (m_currpb) {
		m_currpb->set_active(false);
	}

	pb->set_active(true);
	m_currpb = pb;

	signal_handler_running = false;
}

void Pluginbar::add_plugin_button(Glib::ustring plugin_id, Glib::ustring image_path, Glib::ustring str_label) {
	PluginButton *pb = Gtk::manage(new PluginButton(plugin_id, str_label, image_path));
	pb->signal_realize().connect(boost::bind(&Pluginbar::change_style, this, pb));
	pb->signal_toggled().connect(boost::bind(&Pluginbar::on_pluginbutton_toggled, this, pb));

	button_list[plugin_id] = pb;

	m_vbox->pack_start(*pb, 0, 0);

	show_all();
}

void Pluginbar::change_style(Gtk::Widget *widget) {
	if (!m_refCssProvider) {
		m_refCssProvider = Gtk::CssProvider::create();
		// change background color
		c_normal = get_style_context()->get_background_color(Gtk::STATE_FLAG_NORMAL);
		c_prelight = get_style_context()->get_background_color(Gtk::STATE_FLAG_PRELIGHT);
		darken_color(c_normal, 9);

		css_data = css_data.replace(css_data.find("__COLOR__"), 9, c_normal.to_string().c_str());
		// create the style and apply the style provider
		Glib::ustring css_data2 = ""
				"@define-color bg_color " + c_normal.to_string() + ";"
				"@define-color prelight_color " + c_prelight.to_string() + ";"
				"GtkEventBox {"
				"	background-color: @bg_color;"
				"}"
				"GtkButton:hover, .button:hover, GtkButton:active, .button:active {"
				"	border-image-width: 10 10 10 10;"
				"	border-width: 1;"
				"	border-color: shade (@bg_color, 0.50);"
				"	background-image: -gtk-gradient (linear, left top, left bottom,"
				"		from (shade (@bg_color, 1.3)),"
				"		to (shade (@bg_color, 1.0)));"
				"	outline-color: transparent;"
//				"	border-image-source: -gtk-gradient (linear, left top, left bottom,"
//				"		from (shade (@bg_color, 0.6)),"
//				"		to (shade (@bg_color, 0.7)));"
//				"	box-shadow: inset 0 0 -1 -1 alpha (@bg_color, 0.66);"
				"}"
				""
				"GtkButton, .button {"
				"	border-image-width: 0;"
				"	background-image: -gtk-gradient (linear, left top, left bottom,"
				"		from (shade (@bg_color, 1.0)),"
				"		to (shade (@bg_color, 1.0)));"
//				"	border-image-source: -gtk-gradient (linear, left top, left bottom,"
//				"		from (shade (@bg_color, 0.6)),"
//				"		to (shade (@bg_color, 0.7)));"
				"}"
				"";
		m_refCssProvider->load_from_data(css_data);
}

	widget->get_style_context()->add_provider(m_refCssProvider, 800);
}

void Pluginbar::show_plugin_button(const Glib::ustring &id) {
	if (button_list.find(id) == button_list.end()) {
		return;
	}

	button_list[id]->show_all();
}

void Pluginbar::hide_plugin_button(const Glib::ustring &id) {
	if (button_list.find(id) == button_list.end()) {
		return;
	}

	button_list[id]->hide();
}

// ----------------------------------------------------------------------------------------------------------
// methods to darken background color. with friendly support by planner

void Pluginbar::hls_to_rgb(gdouble &h, gdouble &l, gdouble &s) {
	gdouble hue;
	gdouble m1, m2;
	gdouble r, g, b;

	gdouble &lightness = l;
	gdouble &saturation = s;

	if (lightness <= 0.5)
		m2 = lightness * (1 + saturation);
	else
		m2 = lightness + saturation - lightness * saturation;
	m1 = 2 * lightness - m2;

	if (saturation == 0) {
		h = lightness;
		l = lightness;
		s = lightness;
	} else {
		hue = h + 120;
		while (hue > 360)
			hue -= 360;
		while (hue < 0)
			hue += 360;

		if (hue < 60)
			r = m1 + (m2 - m1) * hue / 60;
		else if (hue < 180)
			r = m2;
		else if (hue < 240)
			r = m1 + (m2 - m1) * (240 - hue) / 60;
		else
			r = m1;

		hue = h;
		while (hue > 360)
			hue -= 360;
		while (hue < 0)
			hue += 360;

		if (hue < 60)
			g = m1 + (m2 - m1) * hue / 60;
		else if (hue < 180)
			g = m2;
		else if (hue < 240)
			g = m1 + (m2 - m1) * (240 - hue) / 60;
		else
			g = m1;

		hue = h - 120;
		while (hue > 360)
			hue -= 360;
		while (hue < 0)
			hue += 360;

		if (hue < 60)
			b = m1 + (m2 - m1) * hue / 60;
		else if (hue < 180)
			b = m2;
		else if (hue < 240)
			b = m1 + (m2 - m1) * (240 - hue) / 60;
		else
			b = m1;

		h = r;
		l = g;
		s = b;
	}
}

void Pluginbar::rgb_to_hls(gdouble &r, gdouble &g, gdouble &b) {
	gdouble min;
	gdouble max;
	gdouble h, l, s;
	gdouble delta;

	gdouble &red = r;
	gdouble &green = g;
	gdouble &blue = b;

	if (red > green) {
		if (red > blue)
			max = red;
		else
			max = blue;

		if (green < blue)
			min = green;
		else
			min = blue;
	} else {
		if (green > blue)
			max = green;
		else
			max = blue;

		if (red < blue)
			min = red;
		else
			min = blue;
	}

	l = (max + min) / 2;
	s = 0;
	h = 0;

	if (max != min) {
		if (l <= 0.5)
			s = (max - min) / (max + min);
		else
			s = (max - min) / (2 - max - min);

		delta = max - min;
		if (red == max)
			h = (green - blue) / delta;
		else if (green == max)
			h = 2 + (blue - red) / delta;
		else if (blue == max)
			h = 4 + (red - green) / delta;

		h *= 60;
		if (h < 0.0)
			h += 360;
	}

	r = h;
	g = l;
	b = s;
}

void Pluginbar::style_shade(Gdk::RGBA &a, Gdk::RGBA &b, gdouble k) {
	gdouble red;
	gdouble green;
	gdouble blue;

	red = a.get_red();
	green = a.get_green();
	blue = a.get_blue();

	rgb_to_hls(red, green, blue);

	green *= k;
	if (green > 1.0)
		green = 1.0;
	else if (green < 0.0)
		green = 0.0;

	blue *= k;
	if (blue > 1.0)
		blue = 1.0;
	else if (blue < 0.0)
		blue = 0.0;

	hls_to_rgb(red, green, blue);

	b.set_red_u(red * 65535.0);
	b.set_green_u(green * 65535.0);
	b.set_blue_u(blue * 65535.0);
}

void Pluginbar::darken_color(Gdk::RGBA &color, gint darken_count) {
	Gdk::RGBA &src(color);
	Gdk::RGBA shaded(color);

	while (darken_count) {
		style_shade(src, shaded, 0.93);
		src = shaded;
		darken_count--;
	}

	color = shaded;
}

}
