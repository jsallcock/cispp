cmake_minimum_required(VERSION 3.10)
project(cispp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall -I -mmacosx-version-min=12.5 -std=c++17 -O3")

list(APPEND includes "${PROJECT_SOURCE_DIR}")
list(APPEND includes "/usr/local/include")
message(STATUS "includes= ${includes}")
message(STATUS "PROJECT_SOURCE_DIR= ${PROJECT_SOURCE_DIR}")

find_library(
    LIB_YAML 
    NAMES libyaml-cpp yaml-cpp libyaml-cpp.a
    HINTS "/usr/local/lib"
)
message(STATUS "LIB_YAML= ${LIB_YAML}")

# LIBS
add_library(maths SHARED "${PROJECT_SOURCE_DIR}/src/maths.cpp")
target_include_directories(maths PUBLIC ${includes})

add_library(material SHARED "${PROJECT_SOURCE_DIR}/src/material.cpp")
target_link_libraries(material PUBLIC ${LIB_YAML})
target_include_directories(material PUBLIC ${includes})

add_library(spectrum SHARED "${PROJECT_SOURCE_DIR}/src/spectrum.cpp")
target_include_directories(spectrum PUBLIC ${includes})

add_library(coherence SHARED "${PROJECT_SOURCE_DIR}/src/coherence.cpp")
target_include_directories(coherence PUBLIC ${includes})

add_library(component SHARED "${PROJECT_SOURCE_DIR}/src/component.cpp")
target_link_libraries(component PUBLIC material)
target_include_directories(component PUBLIC ${includes})

add_library(camera SHARED "${PROJECT_SOURCE_DIR}/src/camera.cpp")
target_link_libraries(camera PUBLIC component)
target_include_directories(camera PUBLIC ${includes})

add_library(instrument SHARED "${PROJECT_SOURCE_DIR}/src/instrument.cpp")
target_link_libraries(instrument PUBLIC component camera)
target_include_directories(instrument PUBLIC ${includes})

# TESTS
add_executable(test_maths "${PROJECT_SOURCE_DIR}/test/test_maths.cpp")
target_link_libraries(test_maths PUBLIC maths)

add_executable(test_material "${PROJECT_SOURCE_DIR}/test/test_material.cpp")
target_link_libraries(test_material PUBLIC material)

add_executable(test_spectrum "${PROJECT_SOURCE_DIR}/test/test_spectrum.cpp")
target_link_libraries(test_spectrum PUBLIC spectrum)

add_executable(test_coherence "${PROJECT_SOURCE_DIR}/test/test_coherence.cpp")
target_link_libraries(test_coherence PUBLIC coherence spectrum maths)

add_executable(test_component "${PROJECT_SOURCE_DIR}/test/test_component.cpp")
target_link_libraries(test_component PUBLIC component)

add_executable(test_camera "${PROJECT_SOURCE_DIR}/test/test_camera.cpp")
target_link_libraries(test_camera PUBLIC camera)

add_executable(test_instrument "${PROJECT_SOURCE_DIR}/test/test_instrument.cpp")
target_link_libraries(test_instrument PUBLIC instrument spectrum)
