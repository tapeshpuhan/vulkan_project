//
// Created by tapesh on 09.07.21.
//
#include "instance.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include "swap_chain.h"
#include "window_surface.h"
namespace vulakn {

constexpr uint32_t c_Width{800};
constexpr uint32_t c_Height{600};
constexpr char *c_Vulkan{"Vulkan"};
constexpr char *c_VulkanAppName{"Triangle"};
constexpr char *c_EngineName{"NoEngine"};
constexpr int8_t c_majorVersion{1};
constexpr int8_t c_minorVersion{0};
constexpr int8_t c_patchVersion{0};
constexpr VkPhysicalDeviceType c_physicalDeviceType{VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU};
constexpr float c_queuePriorities{1.0f};

void QueueFamilyIndices::printInfo()
{
  if(graphicsFamily.has_value())
  std::cout<<" graphicsFamily "<<graphicsFamily.value()<<std::endl;

  if(computeFamily.has_value())
  std::cout<<" computeFamily "<<computeFamily.value()<<std::endl;

  if(presentFamily.has_value())
    std::cout<<" presentFamily "<<presentFamily.value()<<std::endl;
}

static void checkExtensions()
{
  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,nullptr);
  std::vector<VkExtensionProperties> extensions(extensionCount);

  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,extensions.data());
  std::cout <<"available extensions:"<<std::endl;
  for(const auto& extension : extensions) {
    std::cout <<'\t'<< extension.extensionName <<'\n';
  }
}

static std::string printDeviceProperties(VkPhysicalDeviceProperties deviceProperties)
{
  std::stringstream stringBuf;

  stringBuf<<"apiVersion : "<<deviceProperties.apiVersion<<" : deviceName"<<deviceProperties.deviceName;
  stringBuf<<"deviceID : "<<deviceProperties.deviceID<<" deviceType : "<<deviceProperties.deviceType<<" driverVersion : "<<deviceProperties.driverVersion;

  return stringBuf.str();
}

static std::string printDeviceFeatures(VkPhysicalDeviceFeatures deviceFeatures)
{
  std::stringstream stringBuf;

  stringBuf<<"geometryShader : "<<deviceFeatures.geometryShader;

  return stringBuf.str();
}

bool CreateInstance::isDeviceSuitable(VkPhysicalDevice device)
{
  VkPhysicalDeviceProperties deviceProperties;
  VkPhysicalDeviceFeatures deviceFeatures;

  vkGetPhysicalDeviceProperties(device, &deviceProperties);
  vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

  std::cout<<printDeviceProperties(deviceProperties)<<std::endl;
  std::cout<<printDeviceFeatures(deviceFeatures)<<std::endl;

  return (deviceProperties.deviceType == c_physicalDeviceType && deviceFeatures.geometryShader)&& getQueueFamilyIndices(device).graphicsFamily.has_value();
}

// CreateInstance
CreateInstance::CreateInstance():m_windowSurface{std::make_shared<WindowSurface>(this)},m_swapChain{std::make_shared<SwapChain>(this)} {}

CreateInstance::~CreateInstance() { cleanup(); }

void CreateInstance::initialize()
{
  initWindow();
  initVulkan();
}

void CreateInstance::run() {

  mainLoop();
  cleanup();
}

void CreateInstance::initWindow() {
  if (GLFW_TRUE != glfwInit()) {
    std::cout << "Error in GLFW init" << std::endl;
  }
  // These are the parameter pass to the GLFW lib to set before creating window
  // GLFW_NO_API , do not use OPENGL library
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  if (m_window == nullptr) {
    m_window = glfwCreateWindow(c_Width, c_Height, c_Vulkan, nullptr, nullptr);
  }
}

void CreateInstance::createVulkanInstance()
{
  VkApplicationInfo appInfo{};
  VkInstanceCreateInfo createInfo{};
  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions = nullptr;

  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = c_VulkanAppName;
  appInfo.applicationVersion =
      VK_MAKE_VERSION(c_majorVersion, c_minorVersion, c_patchVersion);
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;
  appInfo.pEngineName = c_EngineName;

  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  if (glfwExtensions != nullptr) {
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
  } else {
    std::cerr << "Error in Extensions creation" << std::endl;
  }
  createInfo.enabledLayerCount = 0;

  VkResult result = vkCreateInstance(&createInfo, nullptr, &m_instance);

  if (result != VK_SUCCESS) {
    std::stringstream strError;
    strError << "failed to create instance! code : " << result;

    throw std::runtime_error(strError.str());
  }
}

void CreateInstance::initVulkan() {
  createVulkanInstance();
  m_windowSurface->initialize();
  checkExtensions();
  pickPhysicalDevice();
  createLogicalDevice();
  createQueueHandel();
  createPresentQueue();
  m_swapChain->create();
}

void CreateInstance::mainLoop() {
  while ((m_window != nullptr) && (!glfwWindowShouldClose(m_window))) {
    glfwPollEvents();
  }
}

void CreateInstance::cleanup() {

  if (m_window != nullptr) {
    m_windowSurface->clean();
    vkDestroyDevice(m_logicalDevice, nullptr);
    vkDestroyInstance(m_instance, nullptr);
    glfwDestroyWindow(m_window);
    glfwTerminate();
    m_window = nullptr;
  }

}

void CreateInstance::pickPhysicalDevice()
{
  uint32_t deviceCount = 0;

  vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
  if(0 == deviceCount)
  {
    throw std::runtime_error("Error : No physical device found");
  }
  else
  {
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance,&deviceCount,devices.data());
    std::cout<<"Found Number of Device : "<<deviceCount<<std::endl;
    for(const auto& device : devices)
    {
      if(isDeviceSuitable(device))
        m_physicalDevice = device;
    }
  }
  if(m_physicalDevice == VK_NULL_HANDLE)
    throw std::runtime_error("failed to find a suitable GPU!");
}

void CreateInstance::createLogicalDevice()
{
  QueueFamilyIndices indices = getQueueFamilyIndices(m_physicalDevice);
  VkDeviceQueueCreateInfo queueCreateInfo{};
  queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
  queueCreateInfo.queueCount = 1;
  float priority=c_queuePriorities;
  queueCreateInfo.pQueuePriorities = &priority;

  VkDeviceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.pQueueCreateInfos = &queueCreateInfo;
  createInfo.queueCreateInfoCount = 1;

  VkPhysicalDeviceFeatures deviceFeatures;
  vkGetPhysicalDeviceFeatures(m_physicalDevice, &deviceFeatures);
  createInfo.pEnabledFeatures = &deviceFeatures;

  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

  float queuePriority = 1.0f;
  for (uint32_t queueFamily : uniqueQueueFamilies) {
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamily;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(queueCreateInfo);
  }
  createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
  createInfo.pQueueCreateInfos = queueCreateInfos.data();

  if(vkCreateDevice(m_physicalDevice,&createInfo, nullptr,&m_logicalDevice) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create logical device!");
  }
}

void CreateInstance::createQueueHandel()
{
  QueueFamilyIndices indices = getQueueFamilyIndices(m_physicalDevice);
  vkGetDeviceQueue(m_logicalDevice,indices.graphicsFamily.value(),0,&m_graphicsQueue);
}

VkDevice CreateInstance::getDevice() const
{
  return m_logicalDevice;
}

VkPhysicalDevice CreateInstance::getPhysicalDevice() const
{
  return m_physicalDevice;
}

VkQueue CreateInstance::getGraphicsQueue() const
{
  return m_graphicsQueue;
}

VkInstance CreateInstance::getInstance()const
{
  return m_instance;
}

GLFWwindow* CreateInstance::getWindow()const
{
  return m_window;
}

WindowSurface* CreateInstance::getWindowSurface()const
{
  return m_windowSurface.get();
}

QueueFamilyIndices CreateInstance::getQueueFamilyIndices(VkPhysicalDevice device)
{
  QueueFamilyIndices indices;
  uint32_t queueFamilyCount = 0;

  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,nullptr);
  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device,&queueFamilyCount,queueFamilies.data());
  int index{0};

  for(const auto& queue : queueFamilies)
  {
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, index, m_windowSurface->getSurface(), &presentSupport);
    if (presentSupport)
    {
      indices.presentFamily = index;
    }
    if(queue.queueFlags & VK_QUEUE_GRAPHICS_BIT)
    {
      indices.graphicsFamily = index;
    }
    if(queue.queueFlags & VK_QUEUE_COMPUTE_BIT)
    {
      indices.computeFamily = index;
    }
    index++;
  }

  indices.printInfo();

  return indices;
}

void CreateInstance::createPresentQueue()
{
  QueueFamilyIndices indices = getQueueFamilyIndices(m_physicalDevice);

  vkGetDeviceQueue(m_logicalDevice, indices.presentFamily.value(), 0, &m_presentQueue);
}

} // namespace vulakn