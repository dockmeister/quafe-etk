SET (QUAFE_SKILLTRAINING "")

SET (quafe_skilltraining_NAME quafe-skilltraining)
SET (quafe_skilltraining_VERSION_MAJOR 0)
SET (quafe_skilltraining_VERSION_MINOR 1)
SET (quafe_skilltraining_VERSION_PATCH 0+devel)

CONFIGURE_FILE (skilltraining-config.h.in skilltraining-config.h)

INCLUDE_DIRECTORIES (
	${QUAFE_INCLUDE_DIR}
	${EAPI_INCLUDE_DIRS}
	${GTKMM_INCLUDE_DIRS} 
	${Boost_INCLUDE_DIR}
)

LINK_DIRECTORIES (
	${GTKMM_LIBRARY_DIRS} 
	${Boost_LIBRARY_DIR}
)

SET (quafe_skilltraining_SOURCES
	skilltraining.cc
	characternotebook.cc
	characterpage.cc
)

ADD_LIBRARY (quafe-skilltraining SHARED ${quafe_skilltraining_SOURCES}) 

target_link_libraries(quafe-skilltraining ${GTKMM_LIBRARIES} ${Boost_LIBRARIES})

install(TARGETS quafe-skilltraining DESTINATION share/quafe-etk/plugins)
