/*
 * main.cc
 *
 *  Created on: 03.03.2012
 *      Author: cn
 */



#include <gtkmm.h>
#include <stdlib.h>
#include <iostream>
int main(int argc, char **argv) {
	Gtk::Main kit(argc, argv);
	Gtk::Window *win;
	win = new Gtk::Window;

	Gtk::VBox *q = Gtk::manage(new Gtk::VBox);
	Gtk::Entry *e = Gtk::manage(new Gtk::Entry);
	Gtk::Widget *b = Gtk::manage(new Gtk::Button);

	q->pack_start(*e, Gtk::PACK_SHRINK);
	q->pack_start(*b, Gtk::PACK_SHRINK);
	win->add(*q);



	kit.run(*win);
	delete win;
	return EXIT_SUCCESS;
}
