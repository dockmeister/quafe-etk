/*
 * key.h
 *
 *  Created on: 01.03.2012
 *      Author: cn
 */

#ifndef KEYINFO_H_
#define KEYINFO_H_

#include "eapi-config.h"
#include "basicapi.h"

namespace EAPI {


class KeyInfo : public BasicAPI {
public:
	static KeyInfo * create(ustring keyID, ustring vCode);

public:
	void signal_updated();
	bool update();
	bool force_update();
protected:
	KeyInfo();
	virtual ~KeyInfo();
private:
	Date cached_until;
	bool active;

	ustring keyID;
};


}

#endif /* KEYINFO_H_ */
