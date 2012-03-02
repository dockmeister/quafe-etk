cmake_minimum_required (VERSION 2.6)
project (quafe-etk)


set(quafe_etk_NAME quafe-etk)
set(quafe_etk_VERSION_MAJOR 0)
set(quafe_etk_VERSION_MINOR 1)
set(quafe_etk_VERSION_PATCH 0+devel)
set(quafe_etk_PREFIX ${CMAKE_INSTALL_PREFIX})

configure_file(quafe/quafe-etk.h.in quafe/quafe-etk.h)
configure_file(data/quafe-etk.run.in data/quafe-etk.run)

#finding packages
find_package(PkgConfig)
find_package(Boost)
pkg_check_modules(GTKMM gtkmm-3.0)

add_subdirectory (libs/pugixml)
add_subdirectory (libs/eapi)
add_subdirectory (plugins)
add_subdirectory (quafe)

add_subdirectory (tests EXCLUDE_FROM_ALL)

add_custom_target (local_install)
add_dependencies (local_install quafe plugins)

get_target_property (quafe_loc quafe-etk LOCATION)
add_custom_command (TARGET local_install POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${quafe_loc} ${CMAKE_BINARY_DIR}/data/)
