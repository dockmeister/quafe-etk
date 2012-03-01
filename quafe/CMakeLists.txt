link_directories(${GTKMM_LIBRARY_DIRS} ${Boost_LIBRARY_DIR})
include_directories(. /usr/include/ ../libs/pugixml/ ${GTKMM_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})

add_executable (quafe-bin 
	main.cc application.cc preferences.cc utility.cc
	ui/modulebar.cc ui/dialog_preferences.cc ui/window.cc ui/dialogs.cc
) 

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -O0 -Wall -fmessage-length=0 -pthread -DSEAL_ENABLE")
target_link_libraries(quafe-bin ${GTKMM_LIBRARIES} ${Boost_LIBRARIES} pugixml boost_program_options)

install(TARGETS quafe-bin DESTINATION bin)
