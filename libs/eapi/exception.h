/*
 * exception.h
 *
 *  Created on: 01.03.2012
 *      Author: cn
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <glibmm/exception.h>

namespace EAPI {

class Exception : public Glib::Exception {
public:
	Exception();
	virtual ~Exception();
};

}

#endif /* EXCEPTION_H_ */
