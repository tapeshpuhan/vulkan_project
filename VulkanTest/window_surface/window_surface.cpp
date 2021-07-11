//
// Created by tapesh on 11.07.21.
//
#include "window_surface.h"
#include "instance.h"

namespace vulakn
{
WindowSurface::WindowSurface(CreateInstance* instance):m_instance{instance}
{

}

void WindowSurface::initialize()
{
  createSurface();
}

void WindowSurface::clean()
{
  if(m_instance->getInstance())
    vkDestroySurfaceKHR(m_instance->getInstance(), m_surface, nullptr);
  else
    throw std::runtime_error("instance not created!");
}

void WindowSurface::createSurface()
{
  if(GLFW_TRUE!=glfwVulkanSupported())
  {
    throw std::runtime_error("Vulkan not supported to create window surface");
  }
  if(m_instance->getInstance())
  {
    if (glfwCreateWindowSurface(m_instance->getInstance(), m_instance->getWindow(), nullptr, &m_surface) != VK_SUCCESS) {
      throw std::runtime_error("failed to create window surface!");
    }
  }
  else
    throw std::runtime_error("vulkan Instance is not created !");
}

VkSurfaceKHR WindowSurface::getSurface()const
{
  return m_surface;
}

}