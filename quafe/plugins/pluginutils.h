/*
 * pluginexecption.h
 *
 *  Created on: 17.02.2013
 *      Author: cn
 */

#ifndef PLUGINEXECPTION_H_
#define PLUGINEXECPTION_H_

#include <glibmm/exception.h>

namespace Quafe {

class PluginException : public Glib::Exception {
public:
	PluginException(const Glib::ustring what);
	virtual ~PluginException() throw();

	virtual Glib::ustring what() const;

private:
	Glib::ustring m_what;
};

} /* namespace Quafe */
#endif /* PLUGINEXECPTION_H_ */
