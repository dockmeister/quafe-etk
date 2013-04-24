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

#ifndef EAPI_CONFIG_H_
#define EAPI_CONFIG_H_

#define EAPI_NAME "eapi"
#define EAPI_STRING "eapi 0.1.0+devel"
#define EAPI_VERSION_MAJOR 0
#define EAPI_VERSION_MINOR 1
#define EAPI_VERSION_PATCH 0+devel
#define EAPI_VERSION "0.1.0+devel"
#define EAPI_PREFIX "C:/Program Files (x86)/Project"

#define EAPI_BUILD_RELEASE 0
#define EAPI_HAVE_LOG4CXX 0

#if defined(WIN32) || defined(_WIN32)
#  if defined(BUILDING_LIBEAPI)
#    define EAPI_API  __declspec(dllexport)
#  else
#    define EAPI_API  __declspec(dllimport)
#  endif
#else
#  define EAPI_API
#endif

#define EAPI_DELETE(ptr); if(ptr) {delete ptr; ptr=0;}
#define NUM_MAX_THREADS 3

#endif /* EAPI_CONFIG_H_ */
