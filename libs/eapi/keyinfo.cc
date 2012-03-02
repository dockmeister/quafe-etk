/*
 * key.cc
 *
 *  Created on: 01.03.2012
 *      Author: cn
 */

#include "keyinfo.h"

#include "exception.h"

namespace EAPI {

KeyInfo * KeyInfo::create(ustring keyID, ustring vCode) {
	//throw new Exception();
}

KeyInfo::KeyInfo() :
	BasicAPI("/account/APIKey.xml.aspx", CACHE_STYLE_SHORT, UPDATE_ONSTARTUP & UPDATE_MANUAL) {
}

KeyInfo::~KeyInfo() {
	// TODO Auto-generated destructor stub
}

}
