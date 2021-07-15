//
// Created by tapesh on 13.07.21.
//
#include "swap_chain.h"
#include "instance.h"
#include "window_surface.h"
#include <sstream>
#include <iostream>
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

VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes, const SwapChainConfig& config) {
  for (const auto& availablePresentMode : availablePresentModes) {
    if (availablePresentMode == config.presentMode) {
      return availablePresentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
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


}

}