link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(. ${QUAFE_LIBS_INCLUDE} ${GTKMM_INCLUDE_DIRS} /usr/include/glibmm-2.4)


add_executable (cli-test cli-test.cc)
add_executable (gui-test gui-test.cc)

target_link_libraries (cli-test ${GLIBMM_LIBRARIES} ${GIOMM_LIBRARIES} -lcurl -pthread -lgthread-2.0 -lrt -lglib-2.0 eapi pugixml)
target_link_libraries (gui-test ${GTKMM_LIBRARIES} -lcurl -pthread -lgthread-2.0 -lrt -lglib-2.0 eapi pugixml)
