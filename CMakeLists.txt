cmake_minimum_required(VERSION 3.0.0)

set(PROJECT_NAME mtfind)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(SOURCE "source")
set(INCLUDE "include")

set(HEADER_FILES
"${INCLUDE}/om_source.h"
"${INCLUDE}/om_wait.h"
"${INCLUDE}/om_worker.h"
"${INCLUDE}/om_visualizer.h"
)

set(SOURCE_FILES
"${SOURCE}/om_main.cpp"
"${SOURCE}/om_source.cpp"
"${SOURCE}/om_worker.cpp"
"${SOURCE}/om_visualizer.cpp"
)

project(${PROJECT_NAME} LANGUAGES CXX)
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE})
