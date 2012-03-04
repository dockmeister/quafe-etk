set (PLUGIN_LIST 
	"quafe-skilltraining" 
	"quafe-character"
)

add_custom_target (plugins)
add_custom_target (plugins-fake-install)

FOREACH(PLUGIN ${PLUGIN_LIST})
	add_subdirectory (${PLUGIN})
	add_dependencies (plugins ${PLUGIN})
	
	get_target_property (PLUGIN_loc ${PLUGIN} LOCATION)
	add_custom_command (TARGET plugins-fake-install POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${PLUGIN_loc} ${CMAKE_BINARY_DIR}/data/plugins/)
ENDFOREACH(PLUGIN)

