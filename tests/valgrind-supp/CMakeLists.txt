link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(. ${GTKMM_INCLUDE_DIRS})

add_executable (valgrind-supp main.cc) 

target_link_libraries(valgrind-supp ${GTKMM_LIBRARIES})

# Generate a minimal gtkmm suppression file to use with quafe-etk
add_custom_target (make-valgrind-supp)
add_dependencies (make-valgrind-supp valgrind-supp)

add_custom_command (TARGET make-valgrind-supp POST_BUILD COMMAND ${CMAKE_COMMAND} -E echo making simple valgrind suppression file)
add_custom_command (TARGET make-valgrind-supp POST_BUILD COMMAND 
	G_SLICE=always-malloc G_DEBUG=gc-friendly,resident-modules
	valgrind --leak-check=full --show-reachable=yes --error-limit=no
		--gen-suppressions=all --log-file=minimalraw.log ./valgrind-supp	
)
add_custom_command (TARGET make-valgrind-supp POST_BUILD COMMAND cat ./minimalraw.log | ./parse-valgrind.sh > minimal.supp)

