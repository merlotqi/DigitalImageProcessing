find_package(GTest REQUIRED)

file(GLOB_RECURSE _testfiles ${CMAKE_CURRENT_LIST_DIR}/*_test.cpp CONFIGURE_DEPEND)

add_executable(DpiUnitTest ${_testfiles})
target_link_libraries(DpiUnitTest PRIVATE DigitalImageProcessing GTest::GTest GTest::Main)
enable_testing()

foreach(_testfile ${_testfiles})
    get_filename_component(_testname ${_testfile} NAME_WE)
    string(REPLACE "_test" "" _suite_name ${_testname})
    add_test(
        NAME dpi_test_${_suite_name}
        COMMAND DpiUnitTest --gtest_filter=${_suite_name}.*
    )
endforeach()

unset(_testfiles)