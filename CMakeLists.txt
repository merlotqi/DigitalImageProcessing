cmake_minimum_required(VERSION 3.10)
project(DigitalImageProcessingProject)

find_package(OpenCV CONFIG REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)

set(IMAGE_DATA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/images/")
set(IMAGE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/processed/")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/common/image_path.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/image_path.h)

set(PROCESSED_DIR "${CMAKE_CURRENT_LIST_DIR}/processed")

if(NOT EXISTS ${PROCESSED_DIR})
    message(STATUS "Creating processed directory: ${PROCESSED_DIR}")
    file(MAKE_DIRECTORY ${PROCESSED_DIR})
else()
    message(STATUS "Processed directory already exists: ${PROCESSED_DIR}")
endif()

set(src_files
    common/image.cpp
    common/image.h

    spaceAlgorithm/contrast_enhancement.cpp
    spaceAlgorithm/contrast_enhancement.h
    spaceAlgorithm/histogram_equalization.cpp
    spaceAlgorithm/histogram_equalization.h
    spaceAlgorithm/image_inversion.cpp
    spaceAlgorithm/image_inversion.h
)

add_library(DigitalImageProcessing 
    ${src_files}
)

target_link_libraries(DigitalImageProcessing Eigen3::Eigen ${OpenCV_LIBS})

target_include_directories(DigitalImageProcessing PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/spaceAlgorithm
    ${CMAKE_CURRENT_BINARY_DIR}
)

include(CTest)
enable_testing()

add_subdirectory(tests)

add_subdirectory(examples)

