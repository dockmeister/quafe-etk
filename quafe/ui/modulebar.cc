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

#include "modulebar.h"

#include <boost/bind.hpp>
#include <gtkmm/label.h>
#include <gtkmm/button.h>
#include <gtkmm/image.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/togglebutton.h>

namespace Quafe {

// ----------------------------------------------------------------------------------------
ModuleBar::ModuleBar() :
	button_active(0), signal_handler_running(false) {
	// remove all childs from Gtk::Frame
	remove();

	// creating the Eventbox
	m_eventbox = Gtk::manage(new Gtk::EventBox);
	m_eventbox->signal_realize().connect(boost::bind(&ModuleBar::change_style, this, m_eventbox));

	m_vbox = Gtk::manage(new Gtk::VBox);
	m_vbox->set_border_width(6);

	m_eventbox->add(*m_vbox);
	add(*m_eventbox);

	set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
}

ModuleBar::~ModuleBar() {

}

void ModuleBar::pre_plugin_button_clicked(Gtk::ToggleButton *button_clicked, ustring plugin_id) {
	if (signal_handler_running)
		return;

	signal_handler_running = true;

	if (button_clicked == button_active) {
		button_clicked->set_active(true);
		signal_handler_running = false;
		return;
	}

	if (!action_plugin_requested(plugin_id)) {
		button_clicked->set_active(false);
		signal_handler_running = false;
		return;
	}

	if (button_active) {
		button_active->set_active(false);
	}

	button_clicked->set_active(true);
	button_active = button_clicked;

	signal_handler_running = false;
}

void ModuleBar::add_plugin_button(ustring plugin_id, ustring image_path, ustring str_label) {
	if(button_list.find(plugin_id) != button_list.end()) {
		return;
	}

	Gtk::Image *image = Gtk::manage(new Gtk::Image(image_path));

	PluginButton pb;
	pb.label = Gtk::manage(new Gtk::Label(str_label));
	pb.label->override_color(Gdk::RGBA("white"), Gtk::STATE_FLAG_NORMAL);

	pb.button = Gtk::manage(new Gtk::ToggleButton);
	pb.button->add(*image);
	pb.button->set_relief(Gtk::RELIEF_NONE);
	pb.button->signal_realize().connect(boost::bind(&ModuleBar::change_style, this, pb.button));
	pb.button->signal_toggled().connect(boost::bind(&ModuleBar::pre_plugin_button_clicked, this, pb.button, plugin_id));

	button_list[plugin_id] = pb;

	m_vbox->pack_start(*pb.button, 0, 0);
	m_vbox->pack_start(*pb.label, 0, 0);

	show_all();
}

void ModuleBar::change_style(Gtk::Widget *widget) {
	if (!m_refCssProvider) {
		m_refCssProvider = Gtk::CssProvider::create();
		// change background color
		c_normal = get_style_context()->get_background_color(Gtk::STATE_FLAG_NORMAL);
		darken_color(c_normal, 9);

		// create the style and apply the style provider
		ustring css_data = "@define-color bg_color " + c_normal.to_string() + ";"
			"GtkEventBox {"
			"	background-color: @bg_color;"
			"}"
			"GtkButton:hover {border-radius:1;}"
			"GtkButton, GtkButton:active {"
			"	border-radius:1;"
			"	background-image: -gtk-gradient (linear, left top, left bottom,"
			"		from (shade (@bg_color, 1.1)),"
			"		to (shade (@bg_color, 0.95)));"
			"	border-image-source: -gtk-gradient (linear, left top, left bottom,"
			"		from (shade (@bg_color, 0.6)),"
			"		to (shade (@bg_color, 0.7)));"
			"}"
			"";

		m_refCssProvider->load_from_data(css_data);
	}

	widget->get_style_context()->add_provider(m_refCssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

// ----------------------------------------------------------------------------------------------------------
// methods to darken background color. with friendly support by planner

void ModuleBar::hls_to_rgb(gdouble &h, gdouble &l, gdouble &s) {
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

void ModuleBar::rgb_to_hls(gdouble &r, gdouble &g, gdouble &b) {
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

void ModuleBar::style_shade(Gdk::RGBA &a, Gdk::RGBA &b, gdouble k) {
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

void ModuleBar::darken_color(Gdk::RGBA &color, gint darken_count) {
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