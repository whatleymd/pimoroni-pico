add_library(usermod_gifdec INTERFACE)

target_sources(usermod_gifdec INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/gifdec.c
    ${CMAKE_CURRENT_LIST_DIR}/gifdec.cpp
    ${CMAKE_CURRENT_LIST_DIR}/AnimatedGIF.cpp
)

target_include_directories(usermod_gifdec INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_definitions(usermod_gifdec INTERFACE
    MODULE_GIFDEC_ENABLED=1
)

target_link_libraries(usermod INTERFACE usermod_gifdec)

set_source_files_properties(${CMAKE_CURRENT_LIST_DIR}/AnimatedGIF.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=unused-function")
