link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(. ../../quafe-bin/ ${GTKMM_INCLUDE_DIRS})

add_executable (api_fetcher api_fetcher.cc) 

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -O3 -Wall -fmessage-length=0 -pthread -DSEAL_ENABLE")
target_link_libraries(api_fetcher ${GTKMM_LIBRARIES} -lcurl)
