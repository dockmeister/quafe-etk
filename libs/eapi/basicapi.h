/*
 * eapi_decl.h
 *
 *  Created on: 01.03.2012
 *      Author: cn
 */

#ifndef BASICAPI_H_
#define BASICAPI_H_

#include "eapi-config.h"

#include <glibmm/thread.h>
#include <glibmm/date.h>
#include <pugixml.hpp>
namespace EAPI {
class BasicAPI;

class BasicAPI {
public:
	const CacheStyle cache;
	const UpdatePolicy policy;
	const ustring filename;
	const ustring uri;

protected:
	BasicAPI();
	virtual void parse_result() = 0;
	virtual void signal_updated() = 0;

	Date age;
	Date cached_until;

	pugi::xml_node result;
private:
	virtual void parse_xml_file();
	virtual void parse_stream();

	Mutex lock_;
	boost::function<void ()> callback;

	std::stringstream stream_;

};
}

#endif /* BASICAPI_H_ */
