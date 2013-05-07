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

#ifndef APIOBJECT_H_
#define APIOBJECT_H_

#include <eapi/types.h>
#include <eapi/apiinterface.h>

#include <list>
#include <boost/bind.hpp>
#include <boost/signals2.hpp>

namespace EAPI {

/*
 *
 */
template<class API>
class EAPI_API APIObject : APIInterface {
public:
	struct Event {
		typedef typename API SheetType;
		const API *sheet;
		uint8_t type;
		uint8_t flags;
	};
public:
	typedef typename std::list<API *> List;
	typedef typename std::list<API *>::iterator iterator;
	typedef typename std::list<API *>::const_iterator const_iterator;
	typedef typename boost::signals2::signal<void (const Event&)> sheet_signal_t;
	typedef typename boost::signals2::signal<void (const Event&)>::slot_type sheet_slot_t;
	typedef boost::signals2::connection slot_connection_t;

public:
	virtual ~APIObject() {
	}
	/*!\brief connects a new slot to signal_sheet_added signal
	 *
	 * @param slot
	 * @return the signal connection object
	 */
	static boost::signals2::connection signal_event(const sheet_slot_t &slot) {
		return m_signal_event.connect(slot);
	}

	/*! \brief Finds a specific sheet by id
	 *
	 * @param id
	 * @param api
	 * @return true if a sheet with id was found
	 */
	static bool find(const int id, API *&ptr) {
		return find_if(boost::bind(&APIInterface::id, _1) == id, ptr);
	}

	template<typename _Predicate>
	static bool find_if(_Predicate __pred, API *&ptr);

	static iterator begin();
	static iterator end();
	static void remove(API *);

	virtual void update(const sheet_slot_t &slot) = 0;
protected:
	static API * manage(API *);

	virtual void finish(const APIEvent res);

	slot_connection_t sig_connection;

private:
	/*! \brief Destructor for SheetList<API>
	 *
	 */
	class Destructor {
	public:
		~Destructor() {
			for (auto itr : m_sheet_list) {
				EAPI_DELETE(itr);
			}
			m_sheet_list.clear();
		}
	};
	friend class Destructor;

	static List m_sheet_list;
	static sheet_signal_t m_signal_event;

	static bool once;
};

template<class API>
typename APIObject<API>::sheet_signal_t APIObject<API>::m_signal_event;


template<class API>
typename APIObject<API>::List APIObject<API>::m_sheet_list;

template<class API>
bool APIObject<API>::once = false;

template<class API>
API * APIObject<API>::manage(API *api) {
	if (!once) {
		once = true;
		static Destructor destruct;
	}
	m_sheet_list.push_back(api);

	api->finish(API_EVENT_ADDED);
	return api;
}

template<class API>
void APIObject<API>::finish(const APIEvent res) {
	API *ptr = static_cast<API*>(this);
	m_signal_event({ptr, res, m_status});
}

template<class API>
template<typename _Predicate>
bool APIObject<API>::find_if(_Predicate __pred, API *&ptr) {
	iterator it = std::find_if(m_sheet_list.begin(), m_sheet_list.end(), __pred);

	if (it == m_sheet_list.end())
		return false;

	ptr = (*it);
	return true;
}

template<class API>
void APIObject<API>::remove(API *api) {
	if(api == 0 || m_sheet_list.find(api) == m_sheet_list.end()) {
		return;
	}

	api->finish(API_EVENT_REMOVED);
	m_sheet_list.remove(api);
	EAPI_DELETE(api);
}

template<class API>
typename APIObject<API>::iterator APIObject<API>::begin() {
	return m_sheet_list.begin();
}

template<class API>
typename APIObject<API>::iterator APIObject<API>::end() {
	return m_sheet_list.end();
}

} /* namespace EAPI */
#endif /* APIOBJECT_H_ */
