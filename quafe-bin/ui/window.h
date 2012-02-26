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


#ifndef WINDOW_H_
#define WINDOW_H_

#include <quafe-etk.h>
#include <utility.h>

#include "modulebar.h"

#include <boost/function.hpp>
#include <gtkmm/uimanager.h>
#include <gtkmm/window.h>
#include <gtkmm/widget.h>
#include <gtkmm/actiongroup.h>
#include <gtkmm/action.h>
#include <gtkmm/stock.h>
#include <gtkmm/menubar.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/scrolledwindow.h>

/*
 *
 */
namespace Quafe {

class Window : public Gtk::Window {
	friend class Application;
protected:
	Window();
	~Window();

	gboolean create_window();

	// boost functions
	boost::function<void ()> action_quit;
	boost::function<void ()> action_preferences;

private:
	gboolean create_menubar();
	gboolean create_toolbar();
	gboolean create_modulebar();

	RefPtr<Gtk::UIManager> m_refUIManager;
	RefPtr<Gtk::ActionGroup> m_refActionGroup;

	Gtk::MenuBar *m_ptrMenubar;
	Gtk::Toolbar *m_ptrToolbar;
	Gtk::ScrolledWindow m_refContentFrame;
	ModuleBar *m_ptrModulebar;


};

}

#endif /* WINDOW_H_ */
