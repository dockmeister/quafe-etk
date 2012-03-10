configure_file (eapi-config.h.in eapi-config.h)

link_directories(${GLIBMM_LIBRARY_DIRS} ${GIOMM_LIBRARY_DIRS} ${Boost_LIBRARY_DIR})
include_directories(. ${GLIBMM_INCLUDE_DIRS} ${GIOMM_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} ${QUAFE_LIBS_INCLUDE})

add_library (eapi STATIC
	eapi.cc request.cc exception.cc basicapi.cc 
	sheets/keyinfo.cc sheets/charactersheet.cc
) 

