/*
 * eapi.cc
 *
 *  Created on: 28.02.2012
 *      Author: cn
 */

#include "eapi.h"

#include <glibmm/fileutils.h>

namespace EAPI {

bool Main::init(const ustring &wdir) {
	try {
		if(!Glib::thread_supported()) {
			Glib::thread_init();
		}
	} catch(std::exception &e) {
//		LOG(L_CRITICAL) << "Glib threads are not supported!";
		return false;
	}

	if(!Glib::file_test(wdir, Glib::FILE_TEST_IS_DIR)) {
		// TODO check if dir is writable
		return false;
	}

	Main *eapi = Main::instance();
	eapi->set_working_dir(wdir);

	return true;
}

Main::Main() : m_thread_pool(3) {

}

Main::~Main() {
}

void Main::set_working_dir(const ustring &wdir) {
	m_workdir = wdir;
}

}
