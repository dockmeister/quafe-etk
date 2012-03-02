add_subdirectory (test_api_fetcher)
add_subdirectory (test_eapi)

add_custom_target (tests)
add_dependencies (tests api_fetcher)
add_dependencies (tests eapi)
