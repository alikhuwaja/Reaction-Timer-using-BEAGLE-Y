# Build the HAL (Hardware Abstraction Layer) as a static library
# Headers are accessible via #include "hal/led.h" and "hal/joystick.h"

file(GLOB MY_SOURCES "src/*.c")

add_library(hal STATIC ${MY_SOURCES})

target_include_directories(hal PUBLIC include)

