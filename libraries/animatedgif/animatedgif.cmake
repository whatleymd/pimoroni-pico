add_library(animatedgif
    ${CMAKE_CURRENT_LIST_DIR}/AnimatedGIF.cpp
)

set_source_files_properties(${CMAKE_CURRENT_LIST_DIR}/AnimatedGIF.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=unused-function")

target_include_directories(animatedgif INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(animatedgif pico_stdlib)