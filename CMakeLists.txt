cmake_minimum_required(VERSION 3.10)
project(DigitalImageProcessingProject)

find_package(OpenCV CONFIG REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)

set(src_files
    common/image.cpp
    common/image.h

    spaceAlgorithm/contrast_enhancement.cpp
    spaceAlgorithm/contrast_enhancement.h
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
)

include(CTest)
enable_testing()

add_subdirectory(tests)