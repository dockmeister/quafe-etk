/*
 * log.cc
 *
 *  Created on: 08.02.2012
 *      Author: cn
 */

#include "log.h"

std::ostringstream& Log::print(LOG_LEVEL level) {
	m_level = level;
	return os;
}

Log::~Log() {
	os << std::endl;
	if (m_level < L_NOTICE) {
		fprintf(stderr, "%s: %s", __TIME__, os.str().c_str());
		fflush(stderr);
	} else {
		fprintf(stdout, "%s: %s", __TIME__, os.str().c_str());
		fflush(stdout);
	}
}
