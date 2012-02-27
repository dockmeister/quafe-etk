cmake_minimum_required (VERSION 2.6)
project (quafe-etk)

set(quafe_etk_NAME quafe-etk)
set(quafe_etk_VERSION_MAJOR 0)
set(quafe_etk_VERSION_MINOR 1)
set(quafe_etk_VERSION_PATCH 0+devel)
set(quafe_etk_PREFIX ${CMAKE_INSTALL_PREFIX})

configure_file(quafe-bin/quafe-etk.h.in quafe-bin/quafe-etk.h)

#finding packages
find_package(PkgConfig)
find_package(Boost COMPONENTS thread REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0)

add_subdirectory (libs/pugixml)
add_subdirectory (quafe-bin)
add_subdirectory (quafe-skilltraining)

add_custom_target(fakeinstall 
    ALL
    "${CMAKE_COMMAND}" 
    -D CMAKE_INSTALL_PREFIX:string=tests/
    -P "cmake_install.cmake" 
    DEPENDS quafe-bin) 
