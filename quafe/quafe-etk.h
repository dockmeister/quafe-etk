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


#ifndef QUAFE_ETK_H_
#define QUAFE_ETK_H_

#define QUAFE_NAME "quafe-etk"
#define QUAFE_STRING "quafe-etk 0.1.0+devel"
#define QUAFE_VERSION_MAJOR 0
#define QUAFE_VERSION_MINOR 1
#define QUAFE_VERSION_PATCH 0+devel
#define QUAFE_VERSION "0.1.0+devel"

#define QUAFE_PREFIX "/home/cn/coding/quafe-etk.git/tests"
#define QUAFE_DATADIR "/home/cn/coding/quafe-etk.git/tests/share/quafe-etk/"

#define QUAFE_DELETE(ptr); if(ptr) {delete ptr; ptr=0;}
#if ENABLE_NLS
#	include <glibmm/i18n-lib.h>
#else
#	define _(String) String
#endif

#include <list>
#include <glibmm/ustring.h>
#include <glibmm/refptr.h>

using namespace Glib;

enum API_CHANGE {
	API_ADD = 0,
	API_DELETE = 1,
	API_REFRESH = 2
};

#endif /* QUAFE_ETK_H_ */
