cmake_minimum_required(VERSION 3.19)
project(VulkanTest)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED FATAL_ERROR) # error
find_package( Threads )
find_package(glfw3 REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(instance)
include_directories(window_surface)
#add_definitions(-DVK_VERSION_1_0)

set(instance_src instance/vukan_header.h instance/instance.h instance/instance.cpp )
set(window_surface_src window_surface/window_surface.h window_surface/window_surface.cpp )

add_executable(VulkanTest main.cpp ${instance_src}
                ${window_surface_src})

#target_link_libraries (VulkanTest glfw)
target_link_libraries (VulkanTest ${VULKAN_LIB_LIST})
target_link_libraries (VulkanTest ${SFML_LIBRARIES})
target_link_libraries( VulkanTest ${CMAKE_THREAD_LIBS_INIT} )
target_link_libraries( VulkanTest glfw vulkan)

