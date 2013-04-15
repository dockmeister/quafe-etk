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

#include <eapi/types.h>
#include <eapi/basicapi.h>

#include <list>
#include <boost/bind.hpp>
#include <boost/signals2.hpp>

namespace EAPI {

/*
 *
 */
template<class API>
class SheetList {
public:
	typedef typename std::list<API *> List;
	typedef typename std::list<API *>::iterator iterator;
	typedef typename std::list<API *>::const_iterator const_iterator;
	typedef typename boost::signals2::signal<void(API*)> sheet_signal_t;
	typedef typename boost::signals2::signal<void(API*)>::slot_type sheet_slot_t;
	typedef boost::signals2::connection slot_connection_t;

public:
	virtual ~SheetList() {
	}
	/*!\brief connects a new slot to signal_sheet_added signal
	 *
	 * @param slot
	 * @return the signal connection object
	 */
	static boost::signals2::connection signal_sheet_added(const sheet_slot_t &slot) {
		return m_signal_sheet_added.connect(slot);
	}

	/*!\brief connects a new slot to signal_sheet_removed signal
	 *
	 * @param slot
	 * @return the signal connection object
	 */
	static boost::signals2::connection signal_sheet_removed(const sheet_slot_t &slot) {
		return m_signal_sheet_removed.connect(slot);
	}

	/*!\brief connects a new slot to signal_sheet_updated signal
	 *
	 * @param slot
	 * @return the signal connection object
	 */
	static boost::signals2::connection signal_sheet_updated(const sheet_slot_t &slot) {
		return m_signal_sheet_updated.connect(slot);
	}

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

	static iterator begin();
	static iterator end();
	static void remove(API *);

	virtual void update(const sheet_slot_t &slot) = 0;
protected:
	static API * manage(API *);

	// TODO: bad design 'signal_sheet_updated' should be implemented in BasicAPI
	static sheet_signal_t m_signal_sheet_updated;
	slot_connection_t sig_connection;

private:
	/*! \brief Destructor for SheetList<API>
	 *
	 */
	class Destructor {
	public:
		~Destructor() {
			typename List::iterator it = m_sheet_list.begin();
			for (; it != m_sheet_list.end(); ++it) {
				if((*it) != 0) {
					delete (*it);
					(*it) = 0;
				}
			}
			m_sheet_list.clear();
		}
	};
	friend class Destructor;

	static List m_sheet_list;
	static sheet_signal_t m_signal_sheet_added;
	static sheet_signal_t m_signal_sheet_removed;

	static bool once;
};

template<class API>
typename SheetList<API>::sheet_signal_t SheetList<API>::m_signal_sheet_added;

template<class API>
typename SheetList<API>::sheet_signal_t SheetList<API>::m_signal_sheet_removed;

template<class API>
typename SheetList<API>::sheet_signal_t SheetList<API>::m_signal_sheet_updated;


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

	SheetList<API>::m_signal_sheet_added(api);
	return api;
}

template<class API>
template<typename _Predicate>
bool SheetList<API>::find_if(_Predicate __pred, API *&ptr) {
	iterator it = std::find_if(m_sheet_list.begin(), m_sheet_list.end(), __pred);

	if (it == m_sheet_list.end())
		return false;

	ptr = (*it);
	return true;
}

template<class API>
void SheetList<API>::remove(API *api) {
	if(api != 0) {
		SheetList<API>::m_signal_sheet_removed(api);
		m_sheet_list.remove(api);
		delete api;
		api = 0;
	}
}

template<class API>
typename SheetList<API>::iterator SheetList<API>::begin() {
	return m_sheet_list.begin();
}

template<class API>
typename SheetList<API>::iterator SheetList<API>::end() {
	return m_sheet_list.end();
}

} /* namespace EAPI */
#endif /* SHEETMANAGER_H_ */
