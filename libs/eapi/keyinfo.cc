/*
 * key.cc
 *
 *  Created on: 01.03.2012
 *      Author: cn
 */

#include "keyinfo.h"

#include "exception.h"

namespace EAPI {

void KeyInfo::create(ustring keyID, ustring vCode) {
	throw new Exception();
}

KeyInfo::KeyInfo() {
	cache = CACHE_STYLE_SHORT;
	policy = UPDATE_ONSTARTUP & UPDATE_MANUAL;
	uri = "/account/APIKey.xml.aspx";
}

KeyInfo::~KeyInfo() {
	// TODO Auto-generated destructor stub
}

}
