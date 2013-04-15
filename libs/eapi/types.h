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

#ifndef TYPES_H_
#define TYPES_H_

namespace EAPI {

enum CacheStyle {
	CACHE_STYLE_SHORT,
	CACHE_STYLE_MSHORT,
	CACHE_STYLE_LONG
};

enum UpdateResult {
	API_UPDATE_OK = 1 << 0,
	API_UPDATE_CACHE = 1 << 1,
	API_UPDATE_FAILED = 1 << 2
};

enum StatusFlags {
	API_STATUS_VALID = 1 << 0,
	API_STATUS_CACHED = 1 << 2, //!< API-Data loaded from file and no new version available
	API_STATUS_OUTDATED = 1 << 3, //!< API-Data loaded from file and a new version is available
	API_STATUS_ERROR = 1 << 4 //!< An error occured \see errno & error
};

enum AccessMask {
	CAK_CHARACTERSHEET = 1 << 0,
	CAK_SKILLQUEUE = 1 << 1
};

}

#endif /* TYPES_H_ */
