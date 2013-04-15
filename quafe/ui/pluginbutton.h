/*
 * pluginbutton.h
 *
 *  Created on: 26.02.2013
 *      Author: cn
 */

#ifndef PLUGINBUTTON_H_
#define PLUGINBUTTON_H_

#include <gtkmm/label.h>
#include <gtkmm/togglebutton.h>

namespace Quafe {

class PluginButton : public Gtk::ToggleButton {
public:
	PluginButton(const Glib::ustring &id, const Glib::ustring &txt_label, const Glib::ustring &img_file_path);
	virtual ~PluginButton();

	inline const Glib::ustring& get_plugin_id() const {
		return pid;
	}

private:
	const Glib::ustring pid;
	Gtk::ToggleButton *m_button;
	Gtk::Label *m_label;

};

} /* namespace Quafe */
#endif /* PLUGINBUTTON_H_ */
