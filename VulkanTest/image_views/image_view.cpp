//
// Created by tapesh on 25.07.21.
//
#include "image_view.h"
#include "instance.h"
#include "swap_chain.h"
namespace vulakn
{

ImageViews::ImageViews(CreateInstance* instance):m_instance{instance}
{

}

ImageViews::ImageViews(CreateInstance* instance,const ImageViewConfig& config):m_instance{instance},m_config{config}
{

}

void ImageViews::create()
{
  m_swapChainImageViews.resize(m_instance->getSwapChain()->getSwapChainImages().size());

  size_t index{0};
  for (const auto& swapChainImage : m_instance->getSwapChain()->getSwapChainImages()) {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = swapChainImage;
    createInfo.viewType = m_config.viewType;
    createInfo.format = m_instance->getSwapChain()->getSwapChainFormat();
    createInfo.components.r = m_config.r;
    createInfo.components.g = m_config.b;
    createInfo.components.b = m_config.b;
    createInfo.components.a = m_config.a;
    createInfo.subresourceRange.aspectMask = m_config.aspectMask;
    createInfo.subresourceRange.baseMipLevel = m_config.baseMipLevel;
    createInfo.subresourceRange.levelCount = m_config.levelCount;
    createInfo.subresourceRange.baseArrayLayer = m_config.baseArrayLayer;
    createInfo.subresourceRange.layerCount = m_config.layerCount;

    if (vkCreateImageView(m_instance->getDevice(), &createInfo, nullptr, &m_swapChainImageViews[index++]) != VK_SUCCESS) {
      throw std::runtime_error("failed to create image views!");
    }
  }
}

void ImageViews::clean()
{
  for (auto imageView : m_swapChainImageViews) {
    vkDestroyImageView(m_instance->getDevice(), imageView, nullptr);
  }
  m_swapChainImageViews.clear();
}

}

