CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET (CMAKE_BUILD_TYPE Debug CACHE STRING "default to debug" FORCE)
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fmessage-length=0")
SET (CMAKE_CXX_FLAGS_DEBUG "-O1 -g")
SET (CMAKE_CXX_FLAGS_RELEASE "-O3")

#finding packages
MACRO (Q_FIND_FILE VAR FILE ERROR_MESSAGE)
    FIND_FILE (${VAR} ${FILE} PATH ENV INCLUDE)
    IF (NOT EXISTS ${${VAR}})
        MESSAGE (FATAL_ERROR ${ERROR_MESSAGE})
    ELSE (NOT EXISTS ${${VAR}})
        MESSAGE (STATUS "  ${FILE}")
    ENDIF (NOT EXISTS ${${VAR}})
ENDMACRO ()

FIND_PACKAGE (PkgConfig REQUIRED)
FIND_PACKAGE (Boost REQUIRED COMPONENTS program_options thread)
Q_FIND_FILE("Boost_signals2" "boost/signals2.hpp" "Unable to locate Boosts signals2.hpp. Check your Boost installation")

pkg_check_modules(GIOMM REQUIRED giomm-2.4)
pkg_check_modules(GLIBMM REQUIRED glibmm-2.4)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

# optional packages
pkg_check_modules(LOG4CXX liblog4cxx)
FIND_PACKAGE (Doxygen)

SET (QUAFE_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/quafe")

add_subdirectory (libs/pugixml)
add_subdirectory (libs/eapi)

add_subdirectory (quafe)

add_subdirectory (plugins)


add_subdirectory (tests EXCLUDE_FROM_ALL)

IF (UNIX)
	GET_PROPERTY (quafe-etk_loc TARGET quafe-etk PROPERTY LOCATION)
	GET_PROPERTY (quafe-skilltraining_loc TARGET quafe-skilltraining PROPERTY LOCATION)
	ADD_CUSTOM_TARGET (symlinks
		COMMAND ${CMAKE_COMMAND} -E create_symlink ${quafe-etk_loc} data/quafe-etk
		COMMAND ${CMAKE_COMMAND} -E create_symlink ${quafe-skilltraining_loc} data/plugins/libquafe-skilltraining.so
		DEPENDS quafe quafe-skilltraining
		WORKING_DIRECTORY ${CMAKE_CURRENT_DIR}
		COMMENT Installing symlinks to "${CMAKE_BINARY_DIR}/data/"
	)

	ADD_CUSTOM_TARGET (check_config_includes
		COMMAND find . -name *.h -type f -exec egrep -Hn "'(.*)-config.h'" '{}' "\\;"
		WORKING_DIRECTORY ${CMAKE_CURRENT_DIR}
		COMMENT "Checking for *-config.h includes in header files."
	)
	#ADD_DEPENDENCIES (eapi check_config_includes)
	#ADD_DEPENDENCIES (quafe-etk check_config_includes)
ENDIF (UNIX)

## doxygen support

if(DOXYGEN_FOUND)
	configure_file(${CMAKE_BINARY_DIR}/doxyfile.in ${CMAKE_BINARY_DIR}/doxyfile @ONLY)
	add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/doxyfile WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endif(DOXYGEN_FOUND)

MESSAGE (STATUS "Quafe build summary:")
MESSAGE (STATUS "  Prefix:     ${CMAKE_INSTALL_PREFIX}")
MESSAGE (STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
MESSAGE (STATUS "  Compiler:   ${CMAKE_CXX_COMPILER}")
MESSAGE (STATUS "  CXX Flags:  ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
MESSAGE (STATUS "  Logging:    ${LOG4CXX_FOUND}")
MESSAGE (STATUS "  Build docs: ${DOXYGEN_FOUND}")
