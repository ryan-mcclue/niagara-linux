cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

project(Niagara VERSION 0.0.1 LANGUAGES CXX)

add_executable(
 niagara
 code/main.cpp
)

target_compile_definitions(niagara PRIVATE DEBUG=1 VK_USE_PLATFORM_XLIB_KHR=1)

find_package(Vulkan REQUIRED)
target_link_libraries(niagara PRIVATE ${Vulkan_LIBRARIES})

add_subdirectory(extern/glfw)
target_include_directories(niagara PRIVATE extern/glfw/include)
target_link_libraries(niagara PRIVATE glfw ${GLFW_LIBRARIES})
