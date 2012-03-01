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

extern "C" gboolean dir_exists(const ustring &dir);
extern "C" gboolean make_dir(const ustring &dir);
extern "C" gboolean file_exists(const ustring &dir);

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
