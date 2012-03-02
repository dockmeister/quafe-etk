link_directories(${GTKMM_LIBRARY_DIRS} ${Boost_LIBRARY_DIR})
include_directories(. ../ ../../quafe/ ${GTKMM_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})

add_library (eapi STATIC
	eapi.cc exception.cc
	basicapi.cc keyinfo.cc
) 

target_link_libraries(eapi ${GTKMM_LIBRARIES} ${Boost_LIBRARIES} pugixml)
