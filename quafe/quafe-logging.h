/*
 * quafe-logging.h
 *
 *  Created on: 17.04.2013
 *      Author: cn
 */

#ifndef QUAFE_LOGGING_H_
#define QUAFE_LOGGING_H_

#include <quafe-config.h>
#if QUAFE_HAVE_LOG4CXX
#	include <log4cxx/logger.h>
#	include <glibmm/ustring.h>
#	define QUAFE_DECLARE_STATIC_LOGGER(domain) namespace { \
			::log4cxx::LoggerPtr mStaticLogger(::log4cxx::Logger::getLogger(domain)); \
		}
#	define QUAFE_FORMAT_STRING0(fmt) fmt
#	define QUAFE_FORMAT_STRING1(fmt, A1) Glib::ustring::compose(fmt, A1)
#	define QUAFE_FORMAT_STRING2(fmt, A1, A2) Glib::ustring::compose(fmt, A1, A2)
#	define QUAFE_FORMAT_STRING3(fmt, A1, A2, A3) Glib::ustring::compose(fmt, A1, A2, A3)
#	define QUAFE_FORMAT_STRING4(fmt, A1, A2, A3, A4) Glib::ustring::compose(fmt, A1, A2, A3, A4)
#	define QUAFE_FORMAT_STRING5(fmt, A1, A2, A3, A4, A5) Glib::ustring::compose(fmt, A1, A2, A3, A4, A5)
#	define QUAFE_FORMAT_CHOOSER(ign1, ign2, A1, A2, A3, A4, A5, QUAFE_FORMAT_STRINGX, ...) QUAFE_FORMAT_STRINGX
#	define QUAFE_FORMAT_SIMPLY(args ...) QUAFE_FORMAT_CHOOSER(, ##args, \
									QUAFE_FORMAT_STRING5(args), \
									QUAFE_FORMAT_STRING4(args), \
									QUAFE_FORMAT_STRING3(args), \
									QUAFE_FORMAT_STRING2(args), \
									QUAFE_FORMAT_STRING1(args), \
									QUAFE_FORMAT_STRING0(args))
#	if QUAFE_BUILD_RELEASE
#		define LOG_LEVEL ::log4cxx::Level::INFO_INT
#		define LOG_TRACE(args ...)
#		define LOG_TRACE(args ...)
#	else
#		define LOG_LEVEL ::log4cxx::Level::ALL_INT
#		define LOG_TRACE(args ...) { \
    	    if (LOG4CXX_UNLIKELY(mStaticLogger->isTraceEnabled())) {\
				mStaticLogger->forcedLog(::log4cxx::Level::getTrace(), QUAFE_FORMAT_SIMPLY(args), LOG4CXX_LOCATION); }}
#		define LOG_DEBUG(args ...) { \
    	    if (LOG4CXX_UNLIKELY(mStaticLogger->isDebugEnabled())) {\
				mStaticLogger->forcedLog(::log4cxx::Level::getDebug(), QUAFE_FORMAT_SIMPLY(args), LOG4CXX_LOCATION); }}
#	endif
#	define LOG_INFO(args ...) { \
	        if (mStaticLogger->isInfoEnabled()) {\
	           mStaticLogger->forcedLog(::log4cxx::Level::getInfo(), QUAFE_FORMAT_SIMPLY(args), LOG4CXX_LOCATION); }}
#	define LOG_WARN(args ...) { \
	        if (mStaticLogger->isWarnEnabled()) {\
	           mStaticLogger->forcedLog(::log4cxx::Level::getWarn(), QUAFE_FORMAT_SIMPLY(args), LOG4CXX_LOCATION); }}
#	define LOG_ERROR(args ...) { \
	        if (mStaticLogger->isErrorEnabled()) {\
	           mStaticLogger->forcedLog(::log4cxx::Level::getError(), QUAFE_FORMAT_SIMPLY(args), LOG4CXX_LOCATION); }}
#	define LOG_FATAL(args ...) { \
	        if (mStaticLogger->isFatalEnabled()) {\
				mStaticLogger->forcedLog(::log4cxx::Level::getFatal(), QUAFE_FORMAT_SIMPLY(args), LOG4CXX_LOCATION); }}
#else
#	define QUAFE_DECLARE_STATIC_LOGGER(domain)
#	define LOG_LEVEL
#	define LOG_TRACE(args ...)
#	define LOG_DEBUG(args ...)
#	define LOG_INFO(args ...)
#	define LOG_WARN(args ...)
#	define LOG_ERROR(args ...)
#	define LOG_FATAL(args ...)
#endif /* HAVE_LOG4CXX */

#endif /* QUAFE_LOGGING_H_ */
