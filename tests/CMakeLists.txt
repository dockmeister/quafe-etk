set (TEST_LIST 
	"valgrind-supp" 
	"test-eapi"
)

add_custom_target (tests)

FOREACH(TEST ${TEST_LIST})
	add_subdirectory (${TEST})
	add_dependencies (tests ${TEST})
ENDFOREACH(TEST)

