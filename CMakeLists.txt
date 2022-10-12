cmake_minimum_required(VERSION 3.10)
project(cispp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall -I -mmacosx-version-min=12.5 -std=c++17 -O3")


list(APPEND includes "${PROJECT_SOURCE_DIR}")
list(APPEND includes "/usr/local/include")
message(STATUS "includes= ${includes}")

find_library(
    LIB_YAML 
    NAMES libyaml-cpp yaml-cpp libyaml-cpp.a
    HINTS "/usr/local/lib"
)
message(STATUS "LIB_YAML= ${LIB_YAML}")

# LIBS
add_library(LIB_MATERIAL SHARED "${PROJECT_SOURCE_DIR}/src/material.cpp")
target_link_libraries(LIB_MATERIAL PUBLIC ${LIB_YAML})
target_include_directories(LIB_MATERIAL PUBLIC ${includes})

add_library(LIB_SPECTRUM SHARED "${PROJECT_SOURCE_DIR}/src/spectrum.cpp")
target_include_directories(LIB_SPECTRUM PUBLIC ${includes})

add_library(LIB_COHERENCE SHARED "${PROJECT_SOURCE_DIR}/src/coherence.cpp")
target_include_directories(LIB_COHERENCE PUBLIC ${includes})

add_library(LIB_COMPONENT SHARED "${PROJECT_SOURCE_DIR}/src/component.cpp")
target_link_libraries(LIB_COMPONENT PUBLIC LIB_MATERIAL)
target_include_directories(LIB_COMPONENT PUBLIC ${includes})

add_library(LIB_CAMERA SHARED "${PROJECT_SOURCE_DIR}/src/camera.cpp")
target_link_libraries(LIB_CAMERA PUBLIC LIB_COMPONENT)
target_include_directories(LIB_CAMERA PUBLIC ${includes})

add_library(LIB_INSTRUMENT SHARED "${PROJECT_SOURCE_DIR}/src/instrument.cpp")
target_link_libraries(LIB_INSTRUMENT PUBLIC LIB_COMPONENT LIB_CAMERA)
target_include_directories(LIB_INSTRUMENT PUBLIC ${includes})

# TESTS
add_executable(test_material "${PROJECT_SOURCE_DIR}/test/test_material.cpp")
target_link_libraries(test_material PUBLIC LIB_MATERIAL)

add_executable(test_spectrum "${PROJECT_SOURCE_DIR}/test/test_spectrum.cpp")
target_link_libraries(test_spectrum PUBLIC LIB_SPECTRUM)

add_executable(test_coherence "${PROJECT_SOURCE_DIR}/test/test_coherence.cpp")
target_link_libraries(test_coherence PUBLIC LIB_COHERENCE LIB_SPECTRUM)

add_executable(test_component "${PROJECT_SOURCE_DIR}/test/test_component.cpp")
target_link_libraries(test_component PUBLIC LIB_COMPONENT)

add_executable(test_camera "${PROJECT_SOURCE_DIR}/test/test_camera.cpp")
target_link_libraries(test_camera PUBLIC LIB_CAMERA)

add_executable(test_instrument "${PROJECT_SOURCE_DIR}/test/test_instrument.cpp")
target_link_libraries(test_instrument PUBLIC LIB_INSTRUMENT LIB_SPECTRUM)
