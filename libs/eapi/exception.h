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

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <glibmm/exception.h>

namespace EAPI {

class Exception : public Glib::Exception {
public:
	Exception(const Glib::ustring &error, const Glib::ustring &errno = "");
	virtual ~Exception() throw();

	virtual Glib::ustring what() const;
protected:
	Glib::ustring error_;
	Glib::ustring errno_;
};

class NotOutdatedException : public Exception {
public:
	NotOutdatedException();
};

}

#endif /* EXCEPTION_H_ */
