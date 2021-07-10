#pragma once
#include <GLFW/glfw3.h>
#include <memory>
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
namespace vulakn {
class CreateInstance {
public:

  CreateInstance();
  ~CreateInstance();

  void initialize();
  void run();

  VkDevice getDevice() const;
  VkQueue getGraphicsQueue() const;
  VkInstance getInstance()const;
  GLFWwindow* getWindow()const;

private:
  void initWindow();
  void initVulkan();
  void mainLoop();
  void cleanup();
  void createVulkanInstance();
  void pickPhysicalDevice();
  void createLogicalDevice();
  void createQueueHandel();

  GLFWwindow *m_window = nullptr;
  VkInstance m_instance;
  VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
  VkDevice m_logicalDevice;
  VkQueue m_graphicsQueue;
};
} // namespace vulakn