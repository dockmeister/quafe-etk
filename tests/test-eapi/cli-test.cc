#include "sensitive_data.h"

#include <glibmm.h>
#include <eapi/eapi.h>
#include <eapi/sheets/sheets.h>
#include <iostream>
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH
using namespace EAPI;
EAPI::Main *eapi = 0;
bool running = true;

void print_debug(const Glib::ustring &msg) {
	std::cout << msg << std::endl;
}

void char_callback(EAPI::CharacterSheet *cs) {
	std::cout << "Update finished with" << cs->m_error << std::endl;
	running = false;
}
void keyinfo_callback(EAPI::UpdateResult i, EAPI::KeyInfo *key) {
	std::cout << "Update finished with" << key->m_error << std::endl;

	// now test characters
	KeyInfo::List clist = key->get_character_list();
	std::cout << "Count chars: " << clist.size() << std::endl;
	foreach(KeyInfo::Character c, clist) {
		DLOG("Character found: " << c.id << " - " << c.name);
	}

	CharacterSheet *cs;
	try {
		 cs = CharacterSheet::create((*(clist.begin())).id);
		 cs->update(boost::bind(&char_callback, cs));
	} catch(Exception &e) {
		DLOG(e.what());
		running = false;
	}
}

int main(int argc, char **argv) {
	Glib::init();
	eapi = EAPI::Main::init("data");
	eapi->set_verbose(true);
	eapi->signal_verbose().connect(boost::bind(&print_debug, _1));


	EAPI::KeyInfo *key = EAPI::KeyInfo::create(keyID_, vCode_);
	try {
		key->update(boost::bind(&keyinfo_callback, _1, key));
	} catch(...) {
		keyinfo_callback(EAPI::API_UPDATE_OK, key);
	}

	while(running) {}

	//std::cout << (key->operator []<Glib::ustring>("accessMask")) << std::endl;
	return 1;
}

