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

#ifndef REQUEST_H_
#define REQUEST_H_

#include "eapi-config.h"

#include <queue>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <curl/curl.h>
#include <glibmm/dispatcher.h>


namespace EAPI {

class BasicAPI;

/*! \brief
 *
 */
class Request : private boost::noncopyable {
public:
	//! Curl write function. Writes result into a stringstream
	static size_t write_to_stream(void *contents, size_t size, size_t nmemb, std::stringstream *str);

	//!
	//TODO static size_t write_to_file();

protected:
	//!< Curl debug callback. Push the curl message to the queue and emits the dispatcher signal
	static int curl_debug_callback (CURL *handle, curl_infotype type, char *msg, size_t size, void *);

public:
	Request(BasicAPI *api, update_callback_t callback_, bool verbose = false);
	virtual ~Request();

	//! \brief return whether the request is valid
	bool validate() const;

	//! \brief Perfoms the curl request, if the Request is valid
	void run();

	const CURLcode & get_result() {
		return res;
	}

	const char* get_error() {
		return m_error;
	}

public:
	//!< initialize static Dispatcher and mutex. Has to be called after Glib::init_threads();
	static void init_request_system();

	//!< destroy the static Dispatcher and mutex.
	static void destroy_request_system();

	//!< pop a debug message from the queue
	static const Glib::ustring pop_debug_queue();

	//!< push a debug message to the queue
	static void push_debug_queue(const Glib::ustring &msg);

	//!< pop a debug message from the queue
	static Request * pop_finish_queue();

	//!< push a debug message to the queue
	static void push_finish_queue(Request *req);

	//!< used by
	static Glib::Dispatcher & signal_debug() {
		return *debug_dispatcher;
	}

	//! \brief Notifies EAPI::Main that the Request is finished
	static Glib::Dispatcher & signal_finished() {
		return *debug_dispatcher;
	}

protected:
	CURL *m_curl;
	CURLcode res;
	char m_error[CURL_ERROR_SIZE];

	BasicAPI *m_api;
	update_callback_t callback;
	bool m_verbose;

	static Glib::Dispatcher *debug_dispatcher;
	static Glib::Mutex *lock_debug_queue;
	static std::queue<Glib::ustring> m_debug_queue;

	static Glib::Dispatcher *finish_dispatcher;
	static Glib::Mutex *lock_finish_queue;
	static std::queue<Request *> m_finish_queue;
};

} /* namespace EAPI */
#endif /* REQUEST_H_ */
