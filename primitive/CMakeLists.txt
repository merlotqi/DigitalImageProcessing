file(GLOB_RECURSE _headers ${CMAKE_CURRENT_LIST_DIR}/*.h  CONFIGURE_DEPEND)
file(GLOB_RECURSE _sources ${CMAKE_CURRENT_LIST_DIR}/*.cpp  CONFIGURE_DEPEND)

target_sources(DigitalImageProcessing PUBLIC ${_headers})
target_sources(DigitalImageProcessing PUBLIC ${_sources})

target_include_directories(DigitalImageProcessing PUBLIC ${CMAKE_CURRENT_LIST_DIR})

unset(_headers)
unset(_sources)