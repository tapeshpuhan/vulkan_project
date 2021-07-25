#pragma once
#include "vukan_header.h"
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

class WindowSurface;
class SwapChain;
class ImageViews;

class CreateInstance {
public:

  CreateInstance();
  ~CreateInstance();

  void initialize();
  void run();

  VkDevice getDevice() const;
  VkPhysicalDevice getPhysicalDevice() const;
  VkQueue getGraphicsQueue() const;
  VkInstance getInstance()const;
  GLFWwindow* getWindow()const;
  WindowSurface* getWindowSurface()const;
  QueueFamilyIndices getQueueFamilyIndices(VkPhysicalDevice device);
  SwapChain* getSwapChain()const;

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
  void setupDebugMessenger();

  bool isDeviceSuitable(VkPhysicalDevice device);

  GLFWwindow *m_window = nullptr;
  VkInstance m_instance;
  VkDebugUtilsMessengerEXT m_debugMessenger;
  VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
  VkDevice m_logicalDevice;
  VkQueue m_graphicsQueue;
  VkQueue m_presentQueue;

  std::shared_ptr<WindowSurface> m_windowSurface;
  std::shared_ptr<SwapChain> m_swapChain;
  std::shared_ptr<ImageViews> m_imageViews;
};

} // namespace vulakn