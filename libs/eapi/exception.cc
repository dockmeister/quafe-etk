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

#include <eapi/exception.h>

namespace EAPI {

Exception::Exception(const Glib::ustring &msg, const Glib::ustring &errno) :
	error_(msg), errno_(errno) {
}

Exception::~Exception() throw() {
}

Glib::ustring Exception::what() const {
	return error_;
}

NotOutdatedException::NotOutdatedException() :
		Exception("API is still up to date. Try force_update but it will most likley wont change anything.") {

}
}
