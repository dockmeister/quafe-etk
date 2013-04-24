/**
 *  Quafe - Eve tools for linux.
 *  Copyright (C) 2012 Christoph Nikic
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _EAPI_EXCEPTION_H_
#define _EAPI_EXCEPTION_H_

#include <eapi/eapi-config.h>
#include <typeinfo>
#include <glibmm/exception.h>

namespace EAPI {

class EAPI_API Exception : public Glib::Exception {
public:
	Exception(const Glib::ustring error);
	Exception(const Glib::ustring error, const Glib::ustring errno);
	virtual ~Exception() throw();

	virtual Glib::ustring what() const;
protected:
	const Glib::ustring error_;
	const Glib::ustring errno_;
};

class EAPI_API NotOutdatedException : public Exception {
public:
	NotOutdatedException();
};

template<class T>
class EAPI_API BadValueException : public Exception {
public:
	BadValueException(const Glib::ustring val)
		: Exception(Glib::ustring::compose("Invalid access for value: %1", val)) {
	}
};

template<class T>
class EAPI_API BadValueCastException : public Exception {
public:
	BadValueCastException(const Glib::ustring index, const T value)
		: Exception(Glib::ustring::compose("Invalid cast for value: %1 to %2", index, typeid(T).name())) {
	}
};

}

#endif /* _EAPI_EXCEPTION_H_ */
