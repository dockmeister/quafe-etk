/*
 * pluginexecption.cc
 *
 *  Created on: 17.02.2013
 *      Author: cn
 */

#include "pluginutils.h"

namespace Quafe {

PluginException::PluginException(const Glib::ustring what)
	: m_what(what) {
}

PluginException::~PluginException() throw() {
}

Glib::ustring PluginException::what() const {
	return m_what;
}
} /* namespace Quafe */
