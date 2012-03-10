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

#ifndef SHEETMANAGER_H_
#define SHEETMANAGER_H_

#include "eapi-config.h"
#include "basicapi.h"
#include <algorithm>
#include <list>
#include <boost/bind.hpp>

namespace EAPI {
/*
 *
 */
template<class API>
class SheetList {
public:
	typedef typename std::list<API *> List;
	typedef typename std::list<API *>::iterator Iterator;
public:
	/*! \brief Finds a specific sheet by id
	 *
	 * @param id
	 * @param api
	 * @return true if a sheet with id was found
	 */
	static bool find(const int id, API *&ptr) {
		return find_if(boost::bind(&BasicAPI::id, _1) == id, ptr);
	}

	template<typename _Predicate>
	static bool find_if(_Predicate __pred, API *&ptr);

	static Iterator begin();
	static Iterator end();
	static void remove(API *);
protected:
	static API * manage(API *);

private:
	/*! \brief Destructor for SheetList<API>
	 *
	 */
	class Destructor {
	public:
		~Destructor() {
			typename List::iterator it = m_sheet_list.begin();
			for (; it != m_sheet_list.end(); ++it) {
				EAPI_DELETE(*it);
			}
			m_sheet_list.clear();
		}
	};
	friend class Destructor;

	static List m_sheet_list;

	static bool once;
};

template<class API>
typename SheetList<API>::List SheetList<API>::m_sheet_list;

template<class API>
bool SheetList<API>::once = false;

template<class API>
API * SheetList<API>::manage(API *api) {
	if (!once) {
		once = true;
		static Destructor destruct;
	}
	m_sheet_list.push_back(api);
	return api;
}

template<class API>
template<typename _Predicate>
bool SheetList<API>::find_if(_Predicate __pred, API *&ptr) {
	Iterator it = std::find_if(m_sheet_list.begin(), m_sheet_list.end(),
			__pred);

	if (it == m_sheet_list.end())
		return false;

	//DLOG("address = " << *it << " ref = " << &(*it) << " a val: " (*it)->id);
	ptr = (*it);
	return true;
}

template<class API>
typename SheetList<API>::Iterator SheetList<API>::begin() {
	return m_sheet_list.begin();
}

template<class API>
typename SheetList<API>::Iterator SheetList<API>::end() {
	return m_sheet_list.end();
}

} /* namespace EAPI */
#endif /* SHEETMANAGER_H_ */
