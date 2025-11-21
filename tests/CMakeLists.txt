find_package(GTest CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES "*.cpp")

foreach(src ${SOURCES})
    file(RELATIVE_PATH rel_path "${CMAKE_CURRENT_SOURCE_DIR}" "${src}")
    get_filename_component(rel_dir "${rel_path}" DIRECTORY)
    get_filename_component(file_name "${src}" NAME_WE)

    set(target "${file_name}")

    add_executable(${target} ${src})
    target_link_libraries(${target} PRIVATE 
        GTest::gtest 
        GTest::gtest_main
        DigitalImageProcessing)

    set_target_properties(${target} PROPERTIES 
        FOLDER "Tests/${rel_dir}"
    )
endforeach()