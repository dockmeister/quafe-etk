/*
 * basicapi.cc
 *
 *  Created on: 01.03.2012
 *      Author: cn
 */

#include "basicapi.h"

namespace EAPI {
BasicAPI::BasicAPI(ustring uri_, const CacheStyle cache_, const int policy_) :
	uri(uri_), cache(cache_), policy(policy_){

}

BasicAPI::~BasicAPI() {

}
}
