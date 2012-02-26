link_directories(${GTKMM_LIBRARY_DIRS} ${Boost_LIBRARY_DIR})
include_directories(. ${GTKMM_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})

add_executable (quafe-bin 
	main.cc application.cc preferences.cc utility.cc
	ui/modulebar.cc ui/window_preferences.cc ui/window.cc
) 

target_link_libraries(quafe-bin ${GTKMM_LIBRARIES} ${Boost_LIBRARIES})

install(TARGETS quafe-bin DESTINATION bin)
