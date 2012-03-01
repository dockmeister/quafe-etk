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

#ifndef SINGLETON_H_
#define SINGLETON_H_
#include <iostream> // TODO remove include
#include <boost/noncopyable.hpp>

namespace Quafe {

/*! \brief Singleton implementation
 *
 *	\usage
 *		TODO write usage documentation
 */
template<typename ImplT>
class Singleton : boost::noncopyable {
public:
	static ImplT * instance(); //!< returns the one global instance of Singleton<ImplT>

protected:
	Singleton() {};
	virtual ~Singleton() {};

private:
	static ImplT * m_instance;

	static void init();

	/*!	\brief
	 *
	 */
	class guard {
	public:
		~guard() {
			if (Singleton<ImplT>::m_instance != 0)
				delete Singleton<ImplT>::m_instance;
		}
	};
	friend class guard;
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
template<typename ImplT>
ImplT * Singleton<ImplT>::m_instance = 0;

template<typename ImplT>
ImplT * Singleton<ImplT>::instance() {
	if(m_instance == 0) {
		static guard g;
		m_instance = new ImplT;
		std::cout << "instance created" << std::endl;
	}

	return m_instance;
}
#endif

}

#endif /* SINGLETON_H_ */
