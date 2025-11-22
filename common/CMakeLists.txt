set(IMAGE_DATA_PATH "${CMAKE_SOURCE_DIR}/images/")
set(IMAGE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/processed/")

configure_file(${CMAKE_CURRENT_LIST_DIR}/image_path.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/image_path.h)

set(PROCESSED_DIR "${CMAKE_SOURCE_DIR}/processed")

if(NOT EXISTS ${PROCESSED_DIR})
    message(STATUS "Creating processed directory: ${PROCESSED_DIR}")
    file(MAKE_DIRECTORY ${PROCESSED_DIR})
else()
    message(STATUS "Processed directory already exists: ${PROCESSED_DIR}")
endif()


file(GLOB_RECURSE _headers ${CMAKE_CURRENT_LIST_DIR}/*.h  CONFIGURE_DEPEND)
file(GLOB_RECURSE _sources ${CMAKE_CURRENT_LIST_DIR}/*.cpp  CONFIGURE_DEPEND)

target_sources(DigitalImageProcessing PUBLIC ${_headers})
target_sources(DigitalImageProcessing PUBLIC ${_sources})

target_include_directories(DigitalImageProcessing PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_BINARY_DIR})

unset(_headers)
unset(_sources)