string (TOUPPER "${JSON_ANALYZER_TEST}" JSON_ANALYZER_TEST)
if (NOT "${JSON_ANALYZER_TEST}" STREQUAL "ON")
    message (WARNING "JSON-analyzer functional test is not available. Use '-DJSON_ANALYZER_TEST=ON' param for CMake")
else ()
    set (IF_NAME "eth0") # TODO: Use virtual network interface or at least to make it platform-specific
    set (CHECK_JSON_ANALYZER_SCRIPT "check-json-analyzer.py")
    configure_file ("${CHECK_JSON_ANALYZER_SCRIPT}.in" "${CHECK_JSON_ANALYZER_SCRIPT}")
    add_test (NAME functional_test_json_analyzer COMMAND sudo python ${CHECK_JSON_ANALYZER_SCRIPT})
endif ()
