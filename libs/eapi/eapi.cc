/**
 *  Quafe - Eve tools for linux.
 *  Copyright (C) 2012 Christoph Nikic
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "eapi.h"

#include "request.h"
#include "exception.h"
#include "basicapi.h"
#include "sheetlist.h"

#include <assert.h>
#include <boost/bind.hpp>
#include <glibmm/fileutils.h>


namespace EAPI {

Main * Main::init(const Glib::ustring &wdir) {
	try {
		if (!Glib::thread_supported()) {
			Glib::thread_init();
		}
	} catch (Glib::Error &e) {
		throw Exception("Failed to initialize Glib thread system.");
	}

	if (!Glib::file_test(wdir, Glib::FILE_TEST_IS_DIR)) {
		// TODO check if dir is writable
		// TODO throw eapi::exception
	}

	curl_global_init(CURL_GLOBAL_ALL);

	Request::init_request_system();
	Main *eapi = Main::instance();
	eapi->set_working_dir(wdir);

	return eapi;
}

Main::Main() :
	m_thread_pool(NUM_MAX_THREADS), verbose_(false){

	Request::signal_debug().connect(boost::bind(&Main::curl_debug_recieved, this));
	Request::signal_finished().connect(boost::bind(&Main::request_finished, this));
}

Main::~Main() {
	DLOG("Shutting down EAPI");
	m_thread_pool.shutdown();
	Request::destroy_request_system();
}

void Main::request(BasicAPI *api, update_callback_t callback_) {
	DLOG("debugging: " << verbose_);
	Request *req = new Request(api, callback_, verbose_);

	if (!req->validate()) {
		throw Exception("something went seriously wrong");
	}

	m_thread_pool.push(boost::bind(&Request::run, req));
}

void Main::request_finished() {
	Request *req = Request::pop_finish_queue();
	EAPI_DELETE(req);
}

/********************************************************************************************
 * Methods to control debug output
 */

void Main::curl_debug_recieved() {
	Glib::ustring msg = Request::pop_debug_queue();
	signal_verbose_.emit(msg);
}


}
