link_directories(${GTKMM_LIBRARY_DIRS} ${Boost_LIBRARY_DIR})
include_directories(. ../../quafe ${GTKMM_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})

add_library (quafe-character SHARED
	charinfo.cc
) 

target_link_libraries(quafe-character ${GTKMM_LIBRARIES} ${Boost_LIBRARIES})

install(TARGETS quafe-character DESTINATION share/quafe-etk/plugins)
