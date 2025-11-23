cmake_minimum_required(VERSION 3.10)
project(DigitalImageProcessingProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_DIP_TEST "Build Project DigitalImageProcessing Unit Tests" OFF)
option(BUILD_DIP_EXAMPLE "Build Project DigitalImageProcessing Unit Tests" ON)

find_package(OpenCV CONFIG REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)

add_library(DigitalImageProcessing SHARED)

add_subdirectory(common)
add_subdirectory(spaceAlgorithm)

target_link_libraries(DigitalImageProcessing Eigen3::Eigen ${OpenCV_LIBS})

target_include_directories(DigitalImageProcessing PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/spaceAlgorithm
    ${CMAKE_CURRENT_BINARY_DIR}
)

if (BUILD_DIP_TEST)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()

if (BUILD_DIP_EXAMPLE)
    add_subdirectory(examples)
endif()
