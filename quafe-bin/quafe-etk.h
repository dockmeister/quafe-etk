/*
 * quafe-etk.h
 *
 *  Created on: 07.02.2012
 *      Author: cn
 */

#ifndef QUAFE_ETK_H_
#define QUAFE_ETK_H_

#if HAVE_CONFIG_H
#	include <config.h>
#else
#	define PACKAGE_NAME "gnome-baseball"
#	define PACKAGE_VERSION "0.1+devel"
#	define PACKAGE_STRING "gnome-baseball 0.1+devel"
#	undef ENABLE_NLS
#endif

#if ENABLE_NLS
#	include <glibmm/i18n-lib.h>
#else
#	define _(String) String
#endif

#include <list>

#include <glibmm/ustring.h>
#include <glibmm/refptr.h>

using namespace Glib;

#endif /* QUAFE_ETK_H_ */
