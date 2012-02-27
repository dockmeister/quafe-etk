/*
 * singleton.h
 *
 *  Created on: 27.02.2012
 *      Author: cn
 */

#ifndef SINGLETON_H_
#define SINGLETON_H_

namespace Quafe {

/*! \brief Singleton implementation
 *
 *	\usage
 *		@todo: write usage documentation
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
	static boost::once_flag o_flag;

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
boost::once_flag Singleton<ImplT>::o_flag = BOOST_ONCE_INIT;

template<typename ImplT>
ImplT * Singleton<ImplT>::m_instance = 0;

template<typename ImplT>
void Singleton<ImplT>::init() {
	static guard g;
	m_instance = new ImplT;
}

template<typename ImplT>
ImplT * Singleton<ImplT>::instance() {
	boost::call_once(o_flag, Singleton<ImplT>::init);
	return m_instance;
}
#endif

}

#endif /* SINGLETON_H_ */
