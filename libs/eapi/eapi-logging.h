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
#	if EAPI_BUILD_RELEASE
#		define LOG_LEVEL ::log4cxx::Level::INFO_INT
#		define LOG_TRACE(message)
#		define LOG_DEBUG(message)
#	else
#		define LOG_LEVEL ::log4cxx::Level::ALL_INT
#		define LOG_TRACE(message) { \
    	    if (LOG4CXX_UNLIKELY(mStaticLogger->isTraceEnabled())) {\
				::log4cxx::helpers::MessageBuffer oss_; \
				mStaticLogger->forcedLog(::log4cxx::Level::getTrace(), oss_.str(oss_ << message), LOG4CXX_LOCATION); }}
#		define LOG_DEBUG(message) { \
    	    if (LOG4CXX_UNLIKELY(mStaticLogger->isDebugEnabled())) {\
				::log4cxx::helpers::MessageBuffer oss_; \
				mStaticLogger->forcedLog(::log4cxx::Level::getDebug(), oss_.str(oss_ << message), LOG4CXX_LOCATION); }}
#	endif
#	define LOG_INFO(message) { \
	        if (mStaticLogger->isInfoEnabled()) {\
	           ::log4cxx::helpers::MessageBuffer oss_; \
	           mStaticLogger->forcedLog(::log4cxx::Level::getInfo(), oss_.str(oss_ << message), LOG4CXX_LOCATION); }}
#	define LOG_WARN(message) { \
	        if (mStaticLogger->isWarnEnabled()) {\
	           ::log4cxx::helpers::MessageBuffer oss_; \
	           mStaticLogger->forcedLog(::log4cxx::Level::getWarn(), oss_.str(oss_ << message), LOG4CXX_LOCATION); }}
#	define LOG_ERROR(message) { \
	        if (mStaticLogger->isErrorEnabled()) {\
	           ::log4cxx::helpers::MessageBuffer oss_; \
	           mStaticLogger->forcedLog(::log4cxx::Level::getError(), oss_.str(oss_ << message), LOG4CXX_LOCATION); }}
#	define LOG_FATAL(message, args) { \
	        if (logger->isFatalEnabled()) {\
	           mStaticLogger->forcedLog(::log4cxx::Level::getFatal(), Glib::ustring::compose(message, args), LOG4CXX_LOCATION); }}
#else
#	define EAPI_DECLARE_STATIC_LOGGER(domain)
#	define LOG_LEVEL
#	define LOG_TRACE(message)
#	define LOG_DEBUG(message)
#	define LOG_INFO(message)
#	define LOG_WARN(message)
#	define LOG_ERROR(message)
#	define LOG_FATAL(message)
#endif /* HAVE_LOG4CXX */


#endif /* EAPI_LOGGING_H_ */
