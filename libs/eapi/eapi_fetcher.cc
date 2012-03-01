/*
 * eapi_fetcher.cc
 *
 *  Created on: 28.02.2012
 *      Author: cn
 */

#include "eapi_fetcher.h"

namespace Quafe {

EAPI_Fetcher::EAPI_Fetcher() :
	thread_(0), id_(id) {

}

EAPI_Fetcher::~EAPI_Fetcher() {
	g_return_if_fail_warning(thread_ == 0);
}

void EAPI_Fetcher::launch() {
	thread_ = Glib::Thread::create(boost::mem_fn(this, &EAPI_Fetcher::fetch), true);
}

void EAPI_Fetcher::join() {
	thread_->join();
	thread_ = 0;
}

void fetch() {

}

}
