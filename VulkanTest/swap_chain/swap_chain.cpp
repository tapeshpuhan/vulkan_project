//
// Created by tapesh on 13.07.21.
//
#include "swap_chain.h"
#include "instance.h"
#include "window_surface.h"
#include <sstream>
#include <iostream>
#include <algorithm>
namespace vulakn
{

std::string SwapChainSupportDetails::operator  ()()
{
  std::stringstream  strm;

  strm<<"Capabilities"<<std::endl;
  strm<<"minImageCount : "<<capabilities.minImageCount;
  strm<<" currentExtent.height : "<<capabilities.currentExtent.height<<" currentExtent.width : "<<capabilities.currentExtent.width;
  strm<<" maxImageArrayLayers : "<<capabilities.maxImageArrayLayers;
  strm<<" supportedTransforms : "<<capabilities.supportedTransforms;
  strm<<" supportedCompositeAlpha : "<<capabilities.supportedCompositeAlpha;
  strm<<" supportedUsageFlags : \n"<<capabilities.supportedUsageFlags<<std::endl;

  strm<<"SurfaceFormat list"<<std::endl;
  for(const auto& format : formats)
  {
    strm<<"colorSpace: "<<format.colorSpace;
    strm<<" format : "<<format.format<<std::endl;
  }

  strm<<"PresentModes list"<<std::endl;
  for(const auto& presentMode : presentModes)
  {
    strm<<"presentMode : "<<presentMode<<std::endl;
  }

  return strm.str();
}

static SwapChainSupportDetails querySwapChainSupport(CreateInstance* instance) {
  SwapChainSupportDetails details;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(instance->getPhysicalDevice(), instance->getWindowSurface()->getSurface(), &details.capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(instance->getPhysicalDevice(), instance->getWindowSurface()->getSurface(), &formatCount, nullptr);

  if (formatCount != 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(instance->getPhysicalDevice(), instance->getWindowSurface()->getSurface(), &formatCount, details.formats.data());
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(instance->getPhysicalDevice(), instance->getWindowSurface()->getSurface(), &presentModeCount, nullptr);

  if (presentModeCount != 0) {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(instance->getPhysicalDevice(), instance->getWindowSurface()->getSurface(), &presentModeCount, details.presentModes.data());
  }

  return details;
}

std::optional<VkSurfaceFormatKHR> chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats,const SwapChainConfig& config) {
  for (const auto &availableFormat : availableFormats) {
    if (availableFormat.format == config.format &&
        availableFormat.colorSpace == config.colourSpace) {
      return availableFormat;
    }
  }
  return {};
}

static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes, const SwapChainConfig& config) {
  for (const auto& availablePresentMode : availablePresentModes) {
    if (availablePresentMode == config.presentMode) {
      return availablePresentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities,CreateInstance* instance) {
  if (capabilities.currentExtent.width != UINT32_MAX) {
    return capabilities.currentExtent;
  } else {
    int width, height;
    glfwGetFramebufferSize(instance->getWindow(), &width, &height);

    VkExtent2D actualExtent = {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };

    actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actualExtent;
  }
}

static uint32_t getImageCount(const VkSurfaceCapabilitiesKHR& capabilities)
{
  uint32_t imageCount = capabilities.minImageCount + 1;
  if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
    imageCount = capabilities.maxImageCount;
  }

  return imageCount;
}

SwapChain::SwapChain(CreateInstance* instance):m_instance{instance}
{

}

SwapChain::SwapChain(CreateInstance* instance, const SwapChainConfig& config):m_instance{instance},m_config{config}
{

}

void SwapChain::create()
{
  auto swapChainSupport = querySwapChainSupport(m_instance);
  std::cout<<swapChainSupport();
  auto format = chooseSwapSurfaceFormat(swapChainSupport.formats,m_config);
  if(!format.has_value())
    throw std::runtime_error("swap chain format is not supported");

  auto presentMode = chooseSwapPresentMode(swapChainSupport.presentModes,m_config);
  auto swapExtent = chooseSwapExtent(swapChainSupport.capabilities, m_instance);
  auto imageCount = getImageCount(swapChainSupport.capabilities);

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = m_instance->getWindowSurface()->getSurface();

  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = format.value().format;
  createInfo.imageColorSpace = format.value().colorSpace;
  createInfo.imageExtent = swapExtent;
  createInfo.imageArrayLayers = m_config.imageArrayLayers;
  createInfo.imageUsage = m_config.imageUsage;

  auto indices = m_instance->getQueueFamilyIndices(m_instance->getPhysicalDevice());
  uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

  if (indices.graphicsFamily != indices.presentFamily) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  }

  createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = m_config.compositeAlphaFlagBits;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;

  createInfo.oldSwapchain = VK_NULL_HANDLE;
  if (vkCreateSwapchainKHR(m_instance->getDevice(), &createInfo, nullptr, &m_swapChain) != VK_SUCCESS) {
    throw std::runtime_error("failed to create swap chain!");
  }

  vkGetSwapchainImagesKHR(m_instance->getDevice(), m_swapChain, &imageCount, nullptr);
  m_swapChainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(m_instance->getDevice(), m_swapChain, &imageCount, m_swapChainImages.data());

  m_swapChainImageFormat = format->format;
  m_swapChainExtent = swapExtent;
}

const std::vector<VkImage> SwapChain::getSwapChainImages()const
{
  return m_swapChainImages;
}

const VkFormat SwapChain::getSwapChainFormat()const
{
  return m_swapChainImageFormat;
}

const VkExtent2D SwapChain::getSwapChainExtent()const
{
  return m_swapChainExtent;
}

void SwapChain::clean()
{
  vkDestroySwapchainKHR(m_instance->getDevice(), m_swapChain, nullptr);
}
}