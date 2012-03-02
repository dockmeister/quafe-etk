/*
 * eapi-config.h
 *
 *  Created on: 01.03.2012
 *      Author: cn
 */

#ifndef EAPI_CONFIG_H_
#define EAPI_CONFIG_H_
#include <pugixml/pugixml.hpp>

#include <glibmm/ustring.h>

namespace EAPI {

#define NUM_MAX_THREADS 3

using namespace Glib;

enum CacheStyle {
	CACHE_STYLE_SHORT, CACHE_STYLE_MSHORT, CACHE_STYLE_LONG
};
enum UpdatePolicy {
	UPDATE_ONSTARTUP = 1 << 0,
	UPDATE_AUTO = 1 << 1,
	UPDATE_MANUAL =  1 << 2
};
}
#endif /* EAPI_CONFIG_H_ */
