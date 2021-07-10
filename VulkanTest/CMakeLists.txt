cmake_minimum_required(VERSION 3.19)
project(VulkanTest)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED FATAL_ERROR) # error
find_package( Threads )
find_package(glfw3 REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(instance)
set(instance_src instance/instance.h instance/instance.cpp)

add_executable(VulkanTest main.cpp ${instance_src})

#target_link_libraries (VulkanTest glfw)
target_link_libraries (VulkanTest ${VULKAN_LIB_LIST})
target_link_libraries (VulkanTest ${SFML_LIBRARIES})
target_link_libraries( VulkanTest ${CMAKE_THREAD_LIBS_INIT} )
target_link_libraries( VulkanTest glfw vulkan)

