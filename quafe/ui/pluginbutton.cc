/*
 * pluginbutton.cc
 *
 *  Created on: 26.02.2013
 *      Author: cn
 */

#include <quafe-config.h>

#include "pluginbutton.h"

#include <glibmm/fileutils.h>
#include <gtkmm/image.h>
#include <gtkmm/box.h>

QUAFE_DECLARE_STATIC_LOGGER("EAPI.Ui");

namespace Quafe {

PluginButton::PluginButton(const Glib::ustring &id, const Glib::ustring &txt_label, const Glib::ustring &img_filepath)
		: pid(id), m_button(0), m_label(0) {

	set_manage();

	if (!Glib::file_test(img_filepath, Glib::FILE_TEST_EXISTS)) {
		LOG_WARN("unable to locate plugin image: " << img_filepath);
	}

	Gtk::Image *img = Gtk::manage(new Gtk::Image(img_filepath));
	Gtk::Label *l = Gtk::manage(new Gtk::Label(txt_label));
	Gtk::VBox *box = Gtk::manage(new Gtk::VBox());

	box->pack_start(*img, false, false);
	box->pack_start(*l, false, false);
	add(*box);
}

PluginButton::~PluginButton() {
}

} /* namespace Quafe */
