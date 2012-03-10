cmake_minimum_required (VERSION 2.6)
project (quafe-etk)

set(quafe_etk_NAME quafe-etk)
set(quafe_etk_VERSION_MAJOR 0)
set(quafe_etk_VERSION_MINOR 1)
set(quafe_etk_VERSION_PATCH 0+devel)
set(quafe_etk_PREFIX ${CMAKE_INSTALL_PREFIX})

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fmessage-length=0")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(QUAFE_LIBS_INCLUDE "${CMAKE_BINARY_DIR}/libs" GLOBAL_SCOPE)

configure_file(quafe/quafe-etk.h.in quafe/quafe-etk.h)
configure_file(data/quafe-etk.desktop.in data/quafe-etk.desktop)

#finding packages
find_package(PkgConfig)
find_package(Boost)
pkg_check_modules(GIOMM giomm-2.4)
pkg_check_modules(GLIBMM glibmm-2.4)
pkg_check_modules(GTKMM gtkmm-3.0)




add_subdirectory (libs/pugixml)
add_subdirectory (libs/eapi)
add_subdirectory (plugins)
add_subdirectory (quafe)

add_subdirectory (tests EXCLUDE_FROM_ALL)

add_custom_target (fake-install)
add_dependencies (fake-install quafe-fake-install plugins-fake-install)

## doxygen support
find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(${CMAKE_BINARY_DIR}/doxyfile.in ${CMAKE_BINARY_DIR}/doxyfile @ONLY)
	add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/doxyfile WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endif(DOXYGEN_FOUND)
