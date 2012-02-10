/*
 * log.h
 *
 *  Created on: 08.02.2012
 *      Author: cn
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
