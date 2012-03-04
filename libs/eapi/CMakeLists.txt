link_directories(${GLIBMM_LIBRARY_DIRS} ${Boost_LIBRARY_DIR})
include_directories(. ${GLIBMM_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} ${QUAFE_LIBS_INCLUDE})

add_library (eapi STATIC
	eapi.cc exception.cc
	basicapi.cc keyinfo.cc
) 

