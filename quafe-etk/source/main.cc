/*
 * main.cc
 *
 *  Created on: 05.02.2012
 *      Author: cn
 */

#include <glibmm.h>
#include <gtkmm.h>
#include <iostream>

int main(int argc, char **argv) {
	Gtk::Main kit(argc, argv);
	Glib::RefPtr<Gio::Settings> sett = Gio::Settings::create("apps.quafe-etk");
	sett->set_boolean("my-setting", true);
	g_settings_sync();
	//ssh://git@github.com/dockmeister/quafe-etk.gitkk
}
