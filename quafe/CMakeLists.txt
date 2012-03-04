link_directories(${GTKMM_LIBRARY_DIRS} ${Boost_LIBRARY_DIR})
include_directories(. ${QUAFE_LIBS_INCLUDE} ${GTKMM_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})

add_executable (quafe-etk 
	main.cc application.cc preferences.cc utility.cc pluginmanager.cc
	ui/modulebar.cc ui/dialog_preferences.cc ui/window.cc ui/dialogs.cc
) 

target_link_libraries(quafe-etk ${GTKMM_LIBRARIES} ${Boost_LIBRARIES} eapi pugixml boost_program_options)

install(TARGETS quafe-etk DESTINATION bin)

#fake install
add_custom_target (quafe-fake-install)
get_target_property (quafe_loc quafe-etk LOCATION)
add_custom_command (TARGET quafe-fake-install POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${quafe_loc} ${CMAKE_BINARY_DIR}/data/)
