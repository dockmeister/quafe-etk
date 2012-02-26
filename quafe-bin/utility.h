/**
 *
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

#ifndef LOG_H_
#define LOG_H_
// Log macro
#define LOG(level) Log().print(level)

#include <quafe-etk.h>

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <time.h>
#include <glibmm/exception.h>
#include <boost/bind.hpp>
#include <boost/utility.hpp>
#include <boost/thread/once.hpp>

/*! \brief Singleton implementation
 *
 *	\usage
 *		@todo: write usage documentation
 */
template<typename ImplT>
class Singleton : boost::noncopyable {
public:
	static ImplT * instance(); //!< returns the one global instance of Singleton<ImplT>

protected:
	Singleton() {};
	virtual ~Singleton() {};

private:
	static ImplT * m_instance;
	static boost::once_flag o_flag;

	static void init();

	/*!	\brief
	 *
	 */
	class guard {
	public:
		~guard() {
			if (Singleton<ImplT>::m_instance != 0)
				delete Singleton<ImplT>::m_instance;
		}
	};
	friend class guard;
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
template<typename ImplT>
boost::once_flag Singleton<ImplT>::o_flag = BOOST_ONCE_INIT;

template<typename ImplT>
ImplT * Singleton<ImplT>::m_instance = 0;

template<typename ImplT>
void Singleton<ImplT>::init() {
	static guard g;
	m_instance = new ImplT;
}

template<typename ImplT>
ImplT * Singleton<ImplT>::instance() {
	boost::call_once(o_flag, Singleton<ImplT>::init);
	return m_instance;
}
#endif

//
enum LOG_LEVEL {
	L_CRITICAL = 1 << 0, L_ERROR = 1 << 1, L_WARNING = 1 << 2, L_NOTICE = 1 << 3, L_DEBUG = 1 << 4

};

/** Simple logging class
 * Use the macro LOG(LOG_LEVEL l)
 */
class Log {//: public Singleton<Log> {
public:
	Log() {
	}
	;
	virtual ~Log();

	std::ostringstream& print(LOG_LEVEL level = L_NOTICE);
private:
	Log(const Log&);
	Log& operator =(const Log&);

	LOG_LEVEL m_level;
	std::ostringstream os;
};

namespace Quafe {

/** Quafe Exception class
 * @todo
 */
class Exception: public Glib::Exception {
public:
	Exception(const ustring s);
	virtual ~Exception() throw () {
	}
	;

//virtual
	virtual ustring what() const {
		return s_what;
	}
private:
	const ustring s_what;
};
} // end namspace
#endif /* LOG_H_ */
