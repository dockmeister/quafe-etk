
PROJECT (quafe-etk)

SET (QUAFE_NAME quafe-etk)
SET (QUAFE_VERSION_MAJOR 0)
SET (QUAFE_VERSION_MINOR 1)
SET (QUAFE_VERSION_PATCH 0+devel)
SET (QUAFE_PREFIX ${CMAKE_INSTALL_PREFIX})
SET (QUAFE_BUILD_RELEASE 1)

SET (QUAFE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)

IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
  SET (QUAFE_BUILD_RELEASE 0)
ENDIF (CMAKE_BUILD_TYPE STREQUAL "Debug")

CONFIGURE_FILE (quafe-config.h.in quafe-config.h)
CONFIGURE_FILE (../data/quafe-etk.desktop.in ../data/quafe-etk.desktop)

LINK_DIRECTORIES (
    ${GTKMM_LIBRARY_DIRS} 
    ${Boost_LIBRARY_DIR}
    ${EAPI_LIBRARY_DIRS}
    ${PUGIXML_LIBRARY_DIRS}
)

INCLUDE_DIRECTORIES (
    . 
    ${GTKMM_INCLUDE_DIRS} 
    ${Boost_INCLUDE_DIR}
    ${EAPI_INCLUDE_DIRS}
    ${PUGIXML_INCLUDE_DIRS}
)

SET (quafe_etk_SOURCES
	application.cc
	settings.cc 

#	preferences.cc 
	pluginmanager.cc
	
	# Ui stuff
	ui/applicationwindow.cc 
	ui/pluginbar.cc 
	ui/pluginbutton.cc
	ui/preferences_dialog.cc
	ui/dialogs.cc
	
	# Utilities
	utils/exception.cc
	
	# Plugins
	plugins/pluginbase.cc
	plugins/pluginutils.cc
	plugins/pluginwidget.cc
)

IF (WIN32)
    ADD_EXECUTABLE (quafe-etk WIN32 ${quafe_etk_SOURCES})
ELSE (WIN32)
    ADD_EXECUTABLE (quafe-etk ${quafe_etk_SOURCES})
ENDIF (WIN32) 

TARGET_LINK_LIBRARIES (quafe-etk 
    ${GTKMM_LIBRARIES} 
    ${Boost_LIBRARIES}
    ${EAPI_LIBRARIES}
    ${PUGIXML_LIBRARIES}
    ${LOG4CXX_LIBRARIES}
    dl
)

INSTALL (TARGETS quafe-etk DESTINATION bin)

