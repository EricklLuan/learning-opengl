cmake_minimum_required(VERSION 3.0.0)
project(learning-opengl VERSION 0.1.0)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

file(GLOB SOURCES "src/*.cpp")
add_executable(learning-opengl ${SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_include_directories(learning-opengl PRIVATE "include")

target_link_libraries(learning-opengl PRIVATE glad::glad)
target_link_libraries(learning-opengl PRIVATE glfw)