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
