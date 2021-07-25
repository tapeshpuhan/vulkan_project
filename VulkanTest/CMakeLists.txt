cmake_minimum_required(VERSION 3.19)
project(VulkanTest)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED FATAL_ERROR) # error
find_package( Threads )
find_package(glfw3 REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(instance)
include_directories(window_surface)
include_directories(swap_chain)
include_directories(image_views)

set(instance_src instance/vukan_header.h instance/instance.h instance/instance.cpp )
set(window_surface_src window_surface/window_surface.h window_surface/window_surface.cpp )
set(swap_chain_src swap_chain/swap_chain.h swap_chain/swap_chain.cpp )
set(image_view image_views/image_view.h image_views/image_view.cpp)

add_executable(VulkanTest main.cpp ${instance_src}
                ${swap_chain_src}
                ${window_surface_src}
                ${image_view})

#target_link_libraries (VulkanTest glfw)
target_link_libraries (VulkanTest ${VULKAN_LIB_LIST})
target_link_libraries (VulkanTest ${SFML_LIBRARIES})
target_link_libraries( VulkanTest ${CMAKE_THREAD_LIBS_INIT} )
target_link_libraries( VulkanTest glfw vulkan)

