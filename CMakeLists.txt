cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

project(Niagara VERSION 0.0.1 LANGUAGES CXX)

add_executable(
 niagara
 code/main.cpp
)

add_subdirectory(extern/glfw)
target_include_directories(niagara PRIVATE extern/glfw/include)
target_link_libraries(niagara PRIVATE glfw ${GLFW_LIBRARIES})
