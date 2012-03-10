/*
 * request2.cc
 *
 *  Created on: 05.03.2012
 *      Author: cn
 */

#include "request.h"
#include "basicapi.h"
#include <assert.h>

namespace EAPI {

Glib::Dispatcher *Request::finish_dispatcher = 0;
Glib::Mutex *Request::lock_finish_queue = 0;
std::queue<Request *> Request::m_finish_queue;

Glib::Dispatcher *Request::debug_dispatcher = 0;
Glib::Mutex *Request::lock_debug_queue = 0;
std::queue<Glib::ustring> Request::m_debug_queue;

/****************************************************************************************
 * Curl callbacks
 */

size_t Request::write_to_stream(void *contents, size_t size, size_t nmemb, std::stringstream *str) {
	Glib::ustring buf = Glib::ustring(static_cast<char *>(contents), size * nmemb);

	str->write(buf.c_str(), (std::streamsize) buf.size());

	return size * nmemb;
}

int Request::curl_debug_callback (CURL *handle, curl_infotype type, char *msg, size_t size, void *) {
	if(type == CURLINFO_DATA_IN) {
		return 0;
	}

	Glib::ustring buf = Glib::ustring(static_cast<char *>(msg), size);
	push_debug_queue(buf);
	debug_dispatcher->emit();

	// curl want a zero back
	return 0;
}

/****************************************************************************************
 * Curl callbacks
 */

Request::Request(BasicAPI *api, update_callback_t callback_, bool verbose) :
		m_api(api), callback(callback_), m_verbose(verbose) {

	m_curl = curl_easy_init();
}

Request::~Request() {
	UpdateResult r = API_UPDATE_OK;
	if(res != CURLE_OK)
		r =API_UPDATE_FAILED;
	callback(r);
	curl_easy_cleanup(m_curl);
}

void Request::init_request_system() {
	debug_dispatcher = new Glib::Dispatcher;
	lock_debug_queue = new Glib::Mutex;

	finish_dispatcher = new Glib::Dispatcher;
	lock_finish_queue = new Glib::Mutex;
}

void Request::destroy_request_system() {
	EAPI_DELETE(debug_dispatcher);
	EAPI_DELETE(lock_debug_queue);

	EAPI_DELETE(finish_dispatcher);
	EAPI_DELETE(lock_finish_queue);
}

void Request::run() {
	// check if this request is valid
	if(!validate()) {
		m_api->m_error = "failed to setup request";
		res = CURLE_FAILED_INIT;
		push_finish_queue(this);
	}

	// connect the debug message handler
	if(m_verbose) {
		curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(m_curl, CURLOPT_DEBUGFUNCTION, curl_debug_callback);
		// we dont need any CURLOPT_DEBUGDATA
	}

	Glib::ustring post_fields = m_api->get_postfields();
	Glib::ustring url = "http://api.eveonline.com" + m_api->uri;

	curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(m_curl, CURLOPT_POST, 1L);
	curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, post_fields.c_str());
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, write_to_stream);
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &(m_api->stream_));
	curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(m_curl, CURLOPT_ERRORBUFFER, m_error);

	{
		Glib::Mutex::Lock lock_stream(m_api->lock_stream);
		res = curl_easy_perform(m_curl);

		if(res == CURLE_OK) {
			Glib::Mutex::Lock lock_all(m_api->lock_);
			m_api->parse_stringstream();
		} else {
			if(m_api->m_error.empty()) {
				m_api->m_error = "unknown error occured";
			}
			res = CURLE_SSL_ISSUER_ERROR;
		}
	}

	push_finish_queue(this);
}

bool Request::validate() const {
	if (!m_curl)
		return false;
	return true;
}

/********************************************************************************************
 * Methods to allow interproccess communication
 */

Request * Request::pop_finish_queue() {
	Glib::Mutex::Lock lock(*lock_finish_queue);
	assert(m_finish_queue.empty() == false);
	Request *req = m_finish_queue.front();
	m_finish_queue.pop();
	return req;
}

void Request::push_finish_queue(Request *req) {
	Glib::Mutex::Lock lock(*lock_finish_queue);
	m_finish_queue.push(req);
	finish_dispatcher->emit();
}

const Glib::ustring Request::pop_debug_queue() {
	Glib::Mutex::Lock lock(*lock_debug_queue);
	assert(m_debug_queue.empty() == false);
	Glib::ustring msg = m_debug_queue.front();
	m_debug_queue.pop();
	return msg;
}

void Request::push_debug_queue(const Glib::ustring &msg) {
	Glib::Mutex::Lock lock(*lock_debug_queue);
	m_debug_queue.push(msg);
}


} /* namespace EAPI */
