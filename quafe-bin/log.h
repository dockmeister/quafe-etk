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

#define LOG(level) Log().print(level)

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <time.h>

/*
 *
 */
enum LOG_LEVEL {
	L_CRITICAL = 1 << 0,
	L_ERROR    = 1 << 1,
	L_WARNING  = 1 << 2,
	L_NOTICE   = 1 << 3,
	L_DEBUG    = 1 << 4

};

class Log {
public:
	Log() {};
	virtual ~Log();

	std::ostringstream& print(LOG_LEVEL level = L_NOTICE);
private:
	std::ostringstream os;
	LOG_LEVEL m_level;

	Log(const Log&);
	Log& operator =(const Log&);

};

#endif /* LOG_H_ */
