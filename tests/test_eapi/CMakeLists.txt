link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(. ../../quafe-bin/ ${GTKMM_INCLUDE_DIRS} /usr/include/glibmm-2.4)

add_executable (eapi main.cc) 

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -O3 -Wall -fmessage-length=0 -pthread -DSEAL_ENABLE")
target_link_libraries(eapi ${GTKMM_LIBRARIES} -lcurl -pthread -lgthread-2.0 -lrt -lglib-2.0)  
