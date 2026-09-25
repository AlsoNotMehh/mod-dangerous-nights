# AzerothCore module: mod-dangerous-nights
cmake_minimum_required(VERSION 3.16)

set(MOD_DANGEROUS_NIGHTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_library(mod-dangerous-nights INTERFACE)

target_sources(mod-dangerous-nights INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DangerousNights.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dangerous_nights_loader.cpp
)

target_include_directories(mod-dangerous-nights INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(mod-dangerous-nights INTERFACE
    game
)
