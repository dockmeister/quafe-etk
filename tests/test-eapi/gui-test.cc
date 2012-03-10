#include "sensitive_data.h"

#include <eapi/eapi.h>
#include <eapi/sheets/sheets.h>
#include <iostream>

EAPI::Main *eapi = 0;

#include "eapi_window.h"
#include <gtkmm.h>

class EAPIWindow : public EAPIWindowBase {
public:
	virtual void on_request_button_clicked() {
		EAPI::KeyInfo *key;
		if(!EAPI::KeyInfo::find(keyID_, key)) {
			key = EAPI::KeyInfo::create(keyID_, vCode_);
		}

		if(key->check_status(EAPI::API_STATUS_OUTDATED) > 0) {
			key->update(boost::bind(&EAPIWindow::callback_apikey_updated, this, _1, key));
		}
	}

	void callback_apikey_updated(EAPI::UpdateResult res, EAPI::KeyInfo *key) {
		xml_buffer->insert(xml_buffer->end(), key->get_document());
		//key<Glib::DateTime>["cachedUntil"];
	}

};


int main(int argc, char **argv) {
	Gtk::Main kit(argc, argv);

	EAPIWindow *win = new EAPIWindow;
	eapi = EAPI::Main::init("data");
	eapi->set_verbose(true);
	eapi->signal_verbose().connect(boost::bind(&EAPIWindow::print_debug, win, _1));

	kit.run(*win);
	delete win;

	DLOG("shutdown....");
	return 1;
}

