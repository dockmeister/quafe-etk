link_directories(${GTKMM_LIBRARY_DIRS} ${Boost_LIBRARY_DIR})
include_directories(. ${CMAKE_BINARY_DIR}/libs/ ${GTKMM_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})

add_executable (quafe-etk 
	main.cc application.cc preferences.cc utility.cc pluginmanager.cc
	ui/modulebar.cc ui/dialog_preferences.cc ui/window.cc ui/dialogs.cc
) 

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -O0 -Wall -fmessage-length=0 -pthread -DSEAL_ENABLE")

target_link_libraries(quafe-etk ${GTKMM_LIBRARIES} ${Boost_LIBRARIES} eapi pugixml boost_program_options)

install(TARGETS quafe-etk DESTINATION bin)
