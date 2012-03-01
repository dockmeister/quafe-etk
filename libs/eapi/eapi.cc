/*
 * eapi.cc
 *
 *  Created on: 28.02.2012
 *      Author: cn
 */

#include "eapi.h"

#include "../utility.h"
#include <glibmm/fileutils.h>
namespace EAPI {

bool init(const ustring &wdir) {
	try {
		if(!Glib::thread_supported()) {
			Glib::thread_init();
		}
	} catch(Glib::Exception &e) {
		LOG(L_CRITICAL) << "Glib threads are not supported!";
		return false;
	}

	if(!Glib::file_test(wdir, Glib::FILE_TEST_IS_DIR)) {
		// TODO check if dir is writable
		return false;
	}

	EAPI *eapi = EAPI::instance();
	eapi->set_working_dir(wdir);

	return true;
}

EAPI::EAPI() : m_thread_pool(3) {

}

EAPI::~EAPI() {
	// TODO Auto-generated destructor stub
}

}
