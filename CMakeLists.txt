cmake_minimum_required(VERSION 4.1)
project(AdventOfCode2025)

set(CMAKE_CXX_STANDARD 20)

# Build one executable per top-level .cpp source (e.g. day1.cpp -> target `day1`)
file(GLOB DAY_SOURCES "${CMAKE_SOURCE_DIR}/*.cpp")
foreach(src IN LISTS DAY_SOURCES)
    get_filename_component(name ${src} NAME_WE)
    # skip this CMakeLists or other non-programs if any
    if(name STREQUAL "CMakeLists")
        continue()
    endif()
    add_executable(${name} ${src})
    target_compile_features(${name} PRIVATE cxx_std_20)
    target_include_directories(${name} PRIVATE ${CMAKE_SOURCE_DIR})
endforeach()
