link_directories(${GTKMM_LIBRARY_DIRS} ${Boost_LIBRARY_DIR})
include_directories(. /usr/include/ ../libs/pugixml/ ${GTKMM_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})

add_executable (quafe-bin 
	main.cc application.cc preferences.cc utility.cc
	ui/modulebar.cc ui/window_preferences.cc ui/window.cc ui/dialogs.cc
) 

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -O3 -Wall -fmessage-length=0 -pthread -DSEAL_ENABLE")
target_link_libraries(quafe-bin ${GTKMM_LIBRARIES} ${Boost_LIBRARIES} pugixml)
set_target_properties(quafe-bin PROPERTIES COMPILE_FLAGS -fvisibility=hidden)

install(TARGETS quafe-bin DESTINATION bin)
