/*
 * eapi_fetcher.h
 *
 *  Created on: 28.02.2012
 *      Author: cn
 */

#ifndef EAPI_FETCHER_H_
#define EAPI_FETCHER_H_

#include "eapi-config.h"
#include <curl/curl.h>

namespace EAPI {

struct EAPI_Request {
	ustring keyID;
	ustring vCode;
	ustring url;

	std::stringstream response;
};

class EAPI_Fetcher {
public:
	EAPI_Fetcher(gint id, EAPI_Request &request);
	virtual ~EAPI_Fetcher();

	gint id() const;
	void launch();
	void join();
	gboolean unfinished() const;

	sigc::signal<void>& signal_finished();

protected:
	void fetch();
	static size_t write_data();

private:
	Glib::Thread* thread_;
	gint id_;
	sigc::signal<void> signal_finished_;
	//CURLm *curl;

	ustring url;
	ustring postfields;
	std::stringstream *so;
};

}

#endif /* EAPI_FETCHER_H_ */
