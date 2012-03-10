#ifndef EAPI_WINDOW_H_
#define EAPI_WINDOW_H_

#include <sigc++/sigc++.h>
#include <gtkmm.h>
#include <boost/bind.hpp>

class EAPIWindowBase : public Gtk::Window {
public:
	EAPIWindowBase() {
		set_title("EAPI Test suite");
		set_position(Gtk::WIN_POS_CENTER);
		set_default_size(400,600);

		m_debug_view = Gtk::manage(new Gtk::TextView);
		debug_buffer = Gtk::TextBuffer::create();
		m_debug_view->set_buffer(debug_buffer);
		Gtk::ScrolledWindow *sc1 = Gtk::manage(new Gtk::ScrolledWindow);
		sc1->add(*m_debug_view);

		m_xml_view = Gtk::manage(new Gtk::TextView);
		xml_buffer = Gtk::TextBuffer::create();
		m_xml_view->set_buffer(xml_buffer);
		Gtk::ScrolledWindow *sc2 = Gtk::manage(new Gtk::ScrolledWindow);
		sc2->add(*m_xml_view);

		m_create_button = Gtk::manage(new Gtk::Button("Request"));
		m_create_button->signal_clicked().connect(boost::bind(&EAPIWindowBase::on_create_api_key, this));

		Gtk::Notebook *nb = Gtk::manage(new Gtk::Notebook);
		nb->append_page(*sc1, "Debug");
		nb->append_page(*sc2, "XML Result");

		Gtk::VBox *vbox = Gtk::manage(new Gtk::VBox);
		vbox->pack_start(*m_create_button, Gtk::PACK_SHRINK);
		vbox->pack_end(*nb, Gtk::PACK_EXPAND_WIDGET);

		add(*vbox);
		show_all();
	}

	virtual void on_create_api_key() = 0;
	virtual void on_update_sheet() = 0;

	void print_debug(const Glib::ustring &msg) {
		debug_buffer->insert(debug_buffer->end(), msg);
	}

	Gtk::Entry *m_entry_keyid;
	Gtk::Entry *m_entry_vcode;
	Gtk::Button *m_create_button;

	Gtk::ComboBox *m_combo_characters;
	Gtk::ComboBox *m_combo_sheets;
	Gtk::Button *m_update_button;


	Gtk::TextView *m_debug_view;
	Glib::RefPtr<Gtk::TextBuffer> debug_buffer;

	Gtk::TextView *m_xml_view;
	Glib::RefPtr<Gtk::TextBuffer> xml_buffer;
};


#endif /* EAPI_WINDOW_H_ */
