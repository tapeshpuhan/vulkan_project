#pragma once
#include "vukan_header.h"
#include <vector>
#include <string>
namespace vulakn
{
class CreateInstance;

struct SwapChainSupportDetails
{
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
  std::string operator  ()();
};

struct SwapChainConfig
{
  VkFormat format{VK_FORMAT_B8G8R8A8_SRGB};
  VkColorSpaceKHR colourSpace{VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
  VkPresentModeKHR presentMode{VK_PRESENT_MODE_MAILBOX_KHR};
};

class SwapChain{
public:
  explicit SwapChain(CreateInstance* instance);
  explicit SwapChain(CreateInstance* instance,const SwapChainConfig& config);
  void create();

private:

  CreateInstance* m_instance;
  SwapChainConfig m_config;

};
}