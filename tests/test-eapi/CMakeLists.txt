link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(. ../../quafe-bin/ ${GTKMM_INCLUDE_DIRS} /usr/include/glibmm-2.4)

add_executable (test-eapi main.cc) 

target_link_libraries(test-eapi ${GTKMM_LIBRARIES} -lcurl -pthread -lgthread-2.0 -lrt -lglib-2.0 eapi)  
