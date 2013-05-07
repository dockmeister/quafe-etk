/*
 * eapi-logging.h
 *
 *  Created on: 17.04.2013
 *      Author: cn
 */

#ifndef EAPI_LOGGING_H_
#define EAPI_LOGGING_H_

#include <eapi/eapi-config.h>

#if EAPI_HAVE_LOG4CXX
#	include <log4cxx/logger.h>
#	include <glibmm/ustring.h>
#	define EAPI_DECLARE_STATIC_LOGGER(domain) namespace { \
			::log4cxx::LoggerPtr mStaticLogger(::log4cxx::Logger::getLogger(domain)); \
		}
#	define EAPI_FORMAT_STRING0(fmt) fmt
#	define EAPI_FORMAT_STRING1(fmt, A1) Glib::ustring::compose(fmt, A1)
#	define EAPI_FORMAT_STRING2(fmt, A1, A2) Glib::ustring::compose(fmt, A1, A2)
#	define EAPI_FORMAT_STRING3(fmt, A1, A2, A3) Glib::ustring::compose(fmt, A1, A2, A3)
#	define EAPI_FORMAT_STRING4(fmt, A1, A2, A3, A4) Glib::ustring::compose(fmt, A1, A2, A3, A4)
#	define EAPI_FORMAT_STRING5(fmt, A1, A2, A3, A4, A5) Glib::ustring::compose(fmt, A1, A2, A3, A4, A5)
#	define EAPI_FORMAT_CHOOSER(ign1, ign2, A1, A2, A3, A4, A5, EAPI_FORMAT_STRINGX, ...) EAPI_FORMAT_STRINGX
#	define EAPI_FORMAT_SIMPLY(args ...) EAPI_FORMAT_CHOOSER(, ##args, \
									EAPI_FORMAT_STRING5(args), \
									EAPI_FORMAT_STRING4(args), \
									EAPI_FORMAT_STRING3(args), \
									EAPI_FORMAT_STRING2(args), \
									EAPI_FORMAT_STRING1(args), \
									EAPI_FORMAT_STRING0(args))
#	if EAPI_BUILD_RELEASE
#		define LOG_LEVEL ::log4cxx::Level::INFO_INT
#		define LOG_TRACE(args ...)
#		define LOG_TRACE(args ...)
#	else
#		define LOG_LEVEL ::log4cxx::Level::ALL_INT
#		define LOG_TRACE(args ...) { \
    	    if (LOG4CXX_UNLIKELY(mStaticLogger->isTraceEnabled())) {\
				mStaticLogger->forcedLog(::log4cxx::Level::getTrace(), EAPI_FORMAT_SIMPLY(args), LOG4CXX_LOCATION); }}
#		define LOG_DEBUG(args ...) { \
    	    if (LOG4CXX_UNLIKELY(mStaticLogger->isDebugEnabled())) {\
				mStaticLogger->forcedLog(::log4cxx::Level::getDebug(), EAPI_FORMAT_SIMPLY(args), LOG4CXX_LOCATION); }}
#	endif
#	define LOG_INFO(args ...) { \
	        if (mStaticLogger->isInfoEnabled()) {\
	           mStaticLogger->forcedLog(::log4cxx::Level::getInfo(), EAPI_FORMAT_SIMPLY(args), LOG4CXX_LOCATION); }}
#	define LOG_WARN(args ...) { \
	        if (mStaticLogger->isWarnEnabled()) {\
	           mStaticLogger->forcedLog(::log4cxx::Level::getWarn(), EAPI_FORMAT_SIMPLY(args), LOG4CXX_LOCATION); }}
#	define LOG_ERROR(args ...) { \
	        if (mStaticLogger->isErrorEnabled()) {\
	           mStaticLogger->forcedLog(::log4cxx::Level::getError(), EAPI_FORMAT_SIMPLY(args), LOG4CXX_LOCATION); }}
#	define LOG_FATAL(args ...) { \
	        if (mStaticLogger->isFatalEnabled()) {\
				mStaticLogger->forcedLog(::log4cxx::Level::getFatal(), EAPI_FORMAT_SIMPLY(args), LOG4CXX_LOCATION); }}
#else
#	define EAPI_DECLARE_STATIC_LOGGER(domain)
#	define LOG_LEVEL
#	define LOG_TRACE(args ...)
#	define LOG_DEBUG(args ...)
#	define LOG_INFO(args ...)
#	define LOG_WARN(args ...)
#	define LOG_ERROR(args ...)
#	define LOG_FATAL(args ...)
#endif /* HAVE_LOG4CXX */


#endif /* EAPI_LOGGING_H_ */
