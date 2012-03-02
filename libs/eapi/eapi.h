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

#include "singleton.h"

namespace EAPI {
class BasicAPI;

namespace impl {
/*! \brief
 *
 */
class Request {
public:
	static size_t write_to_stream();

public:
	Request(BasicAPI *api);
	virtual ~Request();

	void run();
	bool unfinished() const;
	sigc::signal<void> & signal_finished();
private:
	sigc::signal<void> signal_finished_;
	ustring url;
		ustring postfields;
		std::stringstream *so;
};
} // end namespace impl




class Main : public Quafe::Singleton<Main> {
	friend class Quafe::Singleton<Main>;
public:
	static bool init(const ustring &dir);
	virtual ~Main();

	virtual void set_working_dir(const ustring &wdir);
private:
	class Fetcher {

	};
	Main();

	Glib::ThreadPool m_thread_pool;
	ustring m_workdir;
};

}

#endif /* EAPI_H_ */
