#pragma once
#include "vukan_header.h"
namespace vulakn
{
  class CreateInstance;

  class WindowSurface
  {
  public:
    explicit WindowSurface(CreateInstance* instance);
    void initialize();
    void clean();
    VkSurfaceKHR getSurface()const;

  private:

    void createSurface();

    CreateInstance* m_instance;
    VkSurfaceKHR m_surface;
  };


}