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

#ifndef EAPI_H_
#define EAPI_H_

#include "eapi-config.h"
#include "exception.h"

#include <boost/signal.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <glibmm/thread.h>
#include <glibmm/threadpool.h>



#include "singleton.h"

namespace EAPI {
class Request;
class BasicAPI;
struct SheetListDestructor;


class Main : public Quafe::Singleton<Main> {
	friend class Quafe::Singleton<Main>;
public:
	/*!
	 *
	 * @param dir
	 * @return
	 */
	static Main* init(const Glib::ustring &dir);

public:

	virtual ~Main();



	virtual void request(BasicAPI *api, update_callback_t callback_);

	virtual void request_finished();


	//!< gets the current working directory
	virtual const Glib::ustring & get_working_dir() {
		return m_workdir;
	}

protected:
	//!< sets the working directory for EAPI. should only be called once
	virtual void set_working_dir(const Glib::ustring &wdir) {
		m_workdir = wdir;
	}
private:
	Main();
	std::list<SheetListDestructor *> m_sheetlist;
	Glib::ThreadPool m_thread_pool;
	Glib::ustring m_workdir;

public:
	//!< Once verbose is enabled connect to signal_verbose() to recieve debug messages
	void set_verbose(bool verbose) {
		verbose_ = verbose;
	}

	//!< Returns whether verbose is enabled
	bool is_verbose() const {
		return verbose_;
	}

	//!< Connect to signal_verbose to recieve debug output
	sigc::signal<void, const Glib::ustring &> & signal_verbose() {
		return signal_verbose_;
	}

public:
	//!< Is connected to the Request::Dispatcher to allow interprocess communication
	void curl_debug_recieved();

	sigc::signal<void, const Glib::ustring &> signal_verbose_;
	bool verbose_;
};

}

#endif /* EAPI_H_ */
