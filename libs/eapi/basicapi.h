/*
 * eapi_decl.h
 *
 *  Created on: 01.03.2012
 *      Author: cn
 */

#ifndef BASICAPI_H_
#define BASICAPI_H_

#include "eapi-config.h"

#include <pugixml/pugixml.hpp>
#include <boost/function.hpp>
#include <glibmm/date.h>
#include <glibmm/thread.h>
namespace EAPI {
class BasicAPI;

class BasicAPI {
public:
	const CacheStyle cache;
	const int policy;
	const ustring filename;
	const ustring uri;

protected:
	BasicAPI(ustring uri_, const CacheStyle cache_, const int policy_);
	virtual ~BasicAPI();
	//virtual void parse_result() = 0;
	//virtual void signal_updated() = 0;

	Date age;
	Date cached_until;

	pugi::xml_node result;
private:
	//virtual void parse_xml_file();
	//virtual void parse_stream();

	//Mutex lock_;
	boost::function<void ()> callback;

	std::stringstream stream_;

};
}

#endif /* BASICAPI_H_ */
