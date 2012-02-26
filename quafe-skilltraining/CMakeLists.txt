link_directories(${GTKMM_LIBRARY_DIRS} ${Boost_LIBRARY_DIR})
include_directories(. ${GTKMM_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})

add_library (quafe-skilltraining 
	skilltraining.cc
) 

target_link_libraries(quafe-skilltraining ${GTKMM_LIBRARIES} ${Boost_LIBRARIES})

install(TARGETS quafe-skilltraining DESTINATION share/quafe-etk/plugins)
