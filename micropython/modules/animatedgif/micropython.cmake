# The following lines are typical in a CMakeLists.txt or similar CMake build configuration for MicroPython integration

set(GIFDEC_SRC
    gifdec.c
    gifdec.cpp
    AnimatedGIF.cpp
)

set(GIFDEC_HEADERS
    gifdec.h
    AnimatedGIF.h
    gif.inl
)

# Specify the target for the GIF decoder
add_library(gifdec STATIC ${GIFDEC_SRC} ${GIFDEC_HEADERS})

# Link necessary libraries
target_link_libraries(gifdec pico_stdlib hardware_pio hardware_dma hardware_interp)

# Include directories for the GIF decoder
target_include_directories(gifdec PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Define the module in MicroPython
set(MICROPYTHON_EXTRA_C_MODULES ${MICROPYTHON_EXTRA_C_MODULES} ${CMAKE_CURRENT_SOURCE_DIR}/gifdec)
