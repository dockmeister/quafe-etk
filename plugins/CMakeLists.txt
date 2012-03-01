add_subdirectory (quafe-skilltraining)
add_subdirectory (quafe-character)

add_custom_target (plugins DEPENDS quafe-skilltraining quafe-character)

add_custom_target(fakeinstall 
    "${CMAKE_COMMAND}" 
    -D CMAKE_INSTALL_PREFIX:string=tests/
    -P "cmake_install.cmake") 
