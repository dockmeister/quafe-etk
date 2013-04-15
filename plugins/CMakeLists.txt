set (PLUGIN_LIST
    "quafe-character" 
	#"quafe-raw"
	#"quafe-skilltraining" 
	
	#"quafe-fitting
	#"quafe-items"
)

add_custom_target (plugins)

FOREACH(PLUGIN ${PLUGIN_LIST})
	add_subdirectory (${PLUGIN})
	add_dependencies (plugins ${PLUGIN})
ENDFOREACH(PLUGIN)

