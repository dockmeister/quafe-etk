/*
 * pluginwidget.cc
 *
 *  Created on: 17.02.2013
 *      Author: cn
 */
#include <quafe-logging.h>

#include "pluginwidget.h"


QUAFE_DECLARE_STATIC_LOGGER("Quafe.Plugin");

namespace Quafe {

PluginWidget::PluginWidget() {

}

PluginWidget::PluginWidget(const Glib::RefPtr<Gtk::Builder> &refGlade)
	: m_refGlade(refGlade) {
}

PluginWidget::~PluginWidget() {
}

bool PluginWidget::add_label(const Glib::ustring label_name) {
	LabelMap::iterator it = m_labels.find(label_name);
	if (it != m_labels.end()) {
		LOG_WARN("duplicate label name: '%1'", label_name);
		return false;
	}

	m_labels[label_name] = Gtk::manage(new Gtk::Label("---"));
	return true;
}

bool PluginWidget::add_label_from_glade(const Glib::ustring label_name) {
	if (!m_refGlade) {
		//TODO: throw Exception?
		LOG_ERROR("failed to add label: Glade is not initialized");
		return false;
	}

	LabelMap::iterator it = m_labels.find(label_name);
	if (it != m_labels.end()) {
		LOG_WARN("duplicate label name: '%1'", label_name);
		return false;
	}

	Gtk::Label *label = 0;
	m_refGlade->get_widget(label_name, label);

	if (!label) {
		LOG_WARN("failed to find label in glade file: '%1'.", label_name);
		return false;
	}

	label->set_text("---");
	m_labels[label_name] = label;
	return true;
}

bool PluginWidget::set_label_text(const Glib::ustring label, const Glib::ustring &txt) {
	LabelMap::iterator it = m_labels.find(label);

	if (it == m_labels.end()) {
		return false;
	}

	it->second->set_text(txt);
	return true;
}
} /* namespace Quafe */
