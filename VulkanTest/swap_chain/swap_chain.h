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

class SwapChain{
public:
  explicit SwapChain(CreateInstance* instance);
  void create();

private:

  CreateInstance* m_instance;

};
}