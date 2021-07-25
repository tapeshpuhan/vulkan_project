#pragma once
#include "vukan_header.h"
#include <vector>
#include <string>

namespace vulakn {


struct ImageViewConfig
{
  VkImageViewType viewType{VK_IMAGE_VIEW_TYPE_2D};
  VkFormat format{VK_FORMAT_UNDEFINED};
  VkComponentSwizzle r{VK_COMPONENT_SWIZZLE_IDENTITY};
  VkComponentSwizzle g{VK_COMPONENT_SWIZZLE_IDENTITY};
  VkComponentSwizzle b{VK_COMPONENT_SWIZZLE_IDENTITY};
  VkComponentSwizzle a{VK_COMPONENT_SWIZZLE_IDENTITY};
  VkImageAspectFlags    aspectMask{VK_IMAGE_ASPECT_COLOR_BIT};
  uint32_t baseMipLevel{0};
  uint32_t levelCount{1};
  uint32_t baseArrayLayer{0};
  uint32_t layerCount{1};
};

class CreateInstance;

class ImageViews
{
public:
  explicit ImageViews(CreateInstance* instance);
  explicit ImageViews(CreateInstance* instance,const ImageViewConfig& config);

  void create();
  void clean();

private:
  CreateInstance* m_instance;
  ImageViewConfig m_config;
  std::vector<VkImageView> m_swapChainImageViews;

};

} // namespace vulakn