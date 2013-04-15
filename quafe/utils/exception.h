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


#include <glibmm/ustring.h>
#include <glibmm/exception.h>

namespace Quafe {

/** Quafe Exception class
 * @todo
 */
class Exception : public Glib::Exception {
public:
	Exception(Glib::ustring s);
	virtual ~Exception() throw () {
	}
	;

//virtual
	virtual Glib::ustring  what() const {
		return s_what;
	}
private:
	Glib::ustring  s_what;
};
} // end namspace
#endif /* LOG_H_ */
