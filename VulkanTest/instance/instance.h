#pragma once
#include "vukan_header.h"
#include "window_surface.h"
#include <memory>
#include <optional>
namespace vulakn {

struct QueueFamilyIndices
{
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> computeFamily;
  std::optional<uint32_t> presentFamily;
  void printInfo();
};

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
  void createPresentQueue();
  QueueFamilyIndices getQueueFamilyIndices(VkPhysicalDevice device);
  bool isDeviceSuitable(VkPhysicalDevice device);

  GLFWwindow *m_window = nullptr;
  VkInstance m_instance;
  VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
  VkDevice m_logicalDevice;
  VkQueue m_graphicsQueue;
  VkQueue m_presentQueue;

  std::shared_ptr<WindowSurface> m_windowSurface;
};

} // namespace vulakn