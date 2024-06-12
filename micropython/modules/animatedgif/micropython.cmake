add_library(gifdec STATIC gifdec.c gifdec.cpp AnimatedGIF.cpp)
target_link_libraries(gifdec pico_stdlib hardware_pio hardware_dma hardware_interp)
