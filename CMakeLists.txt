cmake_minimum_required(VERSION 3.4)
project(slottybeammeup)

find_package(
  SFML 2.3.2 REQUIRED COMPONENTS graphics window audio network system
)

set(ASSETS_PATH "${PROJECT_SOURCE_DIR}/assets" CACHE PATH "Assets path.")
set(
  CMAKE_CXX_FLAGS
  "-std=c++11 -pedantic -Wall -Wextra -Wshadow -Wconversion ${CMAKE_CXX_FLAGS}"
)

configure_file(
  "${PROJECT_SOURCE_DIR}/src/config.hpp.in"
  "${PROJECT_BINARY_DIR}/src/config.hpp"
  @ONLY
)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

include_directories(${SFML_INCLUDE_DIR} "${PROJECT_BINARY_DIR}/src/")
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
