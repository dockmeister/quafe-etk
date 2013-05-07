/*
 * request2.cc
 *
 *  Created on: 05.03.2012
 *      Author: cn
 */

#include <eapi/request.h>
#include <eapi/eapi-logging.h>
#include <eapi/apiinterface.h>

#include <assert.h>

EAPI_DECLARE_STATIC_LOGGER("EAPI");

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

	try {
		Glib::ustring buf = Glib::ustring(static_cast<char *>(msg), size);
		push_debug_queue(buf);
		debug_dispatcher->emit();
	} catch (...) {
		//push_debug_queue(Glib::ustring("Debug msg recieved. Unable to cast"));
		//debug_dispatcher->emit();
	}

	// curl want a zero back
	return 0;
}

/****************************************************************************************
 * Curl callbacks
 */

Request::Request(APIInterface *api, bool verbose) :
		m_api(api), m_verbose(verbose), update_result(API_UPDATE_FAILED), curl_result(CURLE_OK) {

	m_curl = curl_easy_init();
}

Request::~Request() {
	m_api->unlock();
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
		m_api->set_error("failed to setup request");
		curl_result = CURLE_FAILED_INIT;
		push_finish_queue(this);
	}

	do_curl();

	push_finish_queue(this);
}

void Request::run_sync() {
	do_curl();
}

void Request::do_curl() {
	// connect the debug message handler
	if(m_verbose) {
		curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(m_curl, CURLOPT_DEBUGFUNCTION, curl_debug_callback);
	}

	Glib::ustring post_fields = m_api->make_postfields();
	Glib::ustring url = m_api->make_url();

	curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(m_curl, CURLOPT_POST, 1L);
	curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, post_fields.c_str());
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, write_to_stream);
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &(m_api->get_stream()));
	curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(m_curl, CURLOPT_ERRORBUFFER, m_error);

	m_api->lock();
	curl_result = curl_easy_perform(m_curl);

	if(curl_result == CURLE_OK) {
		m_api->parse_stringstream();
	} else {
		m_api->set_error("HTTP Request failed", API_HTTP_FAILED);
	}
	m_api->unlock();
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

int Request::num_finish_queue() {
	Glib::Mutex::Lock lock(*lock_debug_queue);
	return (int)m_finish_queue.size();
}

int Request::num_debug_queue() {
	Glib::Mutex::Lock lock(*lock_debug_queue);
	return (int)m_debug_queue.size();
}


} /* namespace EAPI */
