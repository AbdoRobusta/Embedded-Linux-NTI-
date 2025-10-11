cmake_minimum_required(VERSION 3.12)
project(gpio_buttons_leds_oop VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable and include all source files
add_executable(button
    main.cpp
    button.cpp
)

# Include current directory for header files
target_include_directories(button PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Install target
install(TARGETS button RUNTIME DESTINATION bin)
