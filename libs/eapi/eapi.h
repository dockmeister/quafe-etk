/*
 * eapi.h
 *
 *  Created on: 28.02.2012
 *      Author: cn
 */

#ifndef EAPI_H_
#define EAPI_H_

#include "eapi-config.h"

#include <glibmm/thread.h>
#include <glibmm/threadpool.h>

#include "eapi_fetcher.h"
#include "../include/singleton.h"

namespace EAPI {
class BasicAPI;

static bool init(const ustring &dir);

class EAPI : public Quafe::Singleton<EAPI> {
	friend class Quafe::Singleton<EAPI>;
	friend bool init(const ustring &wdir);
public:
	virtual ~EAPI();

	virtual void set_working_dir(const ustring &wdir);
private:
	class Fetcher {

	};
	EAPI();

	Glib::ThreadPool m_thread_pool;
	ustring wdir;
};

}

#endif /* EAPI_H_ */
