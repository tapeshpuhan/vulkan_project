#include "instance.h"
#include <memory>
#include <iostream>
int main() {
//    glfwInit();
//
//    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
//    GLFWwindow* window = glfwCreateWindow(800, 600, "My Vulkan window", nullptr, nullptr);
//
//    uint32_t extensionCount = 0;
//    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
//
//    std::cout << extensionCount << " extensions supported\n";
//
//    glm::mat4 matrix;
//    glm::vec4 vec;
//    auto test = matrix * vec;
//
//    while(!glfwWindowShouldClose(window)) {
//        glfwPollEvents();
//    }
//
//    glfwDestroyWindow(window);
//
//    glfwTerminate();

    try
    {
        auto instance = std::make_shared<vulakn::CreateInstance>();
        instance->initialize();
        instance->run();
    } catch (const std::exception& error) {
      std::cerr<<"Error : "<<error.what()<<std::endl;
    }

    return VK_SUCCESS;
}