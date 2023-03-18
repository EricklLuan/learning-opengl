cmake_minimum_required(VERSION 3.0.0)
project(learning-opengl VERSION 0.1.0)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_executable(learning-opengl 
	"src/main.cpp" 
	"include/shader.hpp"
	"src/shader.cpp" 
	"include/texture.hpp"
	"src/texture.cpp" 
	"include/camera.hpp" 
	"src/camera.cpp"
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_include_directories(learning-opengl PRIVATE "include")

target_link_libraries(learning-opengl PRIVATE glad::glad)
target_link_libraries(learning-opengl PRIVATE glfw)