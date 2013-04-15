/*
 * pageskilltraining.h
 *
 *  Created on: 17.02.2013
 *      Author: cn
 */

#ifndef CHARACTERPAGE_H_
#define CHARACTERPAGE_H_

#include <plugins/pluginwidget.h>
#include <gtkmm/paned.h>
#include <gtkmm/builder.h>
#include <gtkmm/label.h>

namespace Quafe {
	namespace Plugin {

class CharacterPage : public PluginWidget, public Gtk::Paned {
public:
	CharacterPage(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
	virtual ~CharacterPage();

private:
};

	}
}
#endif /* CHARACTERPAGE_H_ssssss */
