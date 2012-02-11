/*
 * window.h
 *
 *  Created on: 08.02.2012
 *      Author: cn
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include <quafe-etk.h>

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
	boost::function<void ()> on_action_file_quit;
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
