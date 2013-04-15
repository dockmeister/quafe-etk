/*
 * pluginwidget.h
 *
 *  Created on: 17.02.2013
 *      Author: cn
 */

#ifndef PLUGINWIDGET_H_
#define PLUGINWIDGET_H_

#include "pluginutils.h"

#include <preferences.h>
#include <boost/bind.hpp>

#include <glibmm/miscutils.h>
#include <glibmm/fileutils.h>
#include <gtkmm/builder.h>
#include <gtkmm/label.h>

namespace Quafe {

class PluginWidget {
public:
	PluginWidget();
	PluginWidget(Glib::RefPtr<Gtk::Builder> &refGlade);
	virtual ~PluginWidget();

	bool add_label(const Glib::ustring label_name);
	bool add_label_from_glade(const Glib::ustring label_name);
	bool set_label_text(const Glib::ustring label_name, const Glib::ustring &txt);

protected:
	typedef std::map<Glib::ustring, Gtk::Label *> LabelMap;
	LabelMap m_labels;

	Glib::RefPtr<Gtk::Builder> m_refGlade;
private:

};

} /* namespace Quafe */
#endif /* PLUGINWIDGET_H_ */
