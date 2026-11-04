#include "application.hpp"

void Application::initialise_window() {
    if (!glfwInit()) {
        throw std::runtime_error("Failure to initialise GLFW.");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

    if (!window) {
        throw std::runtime_error("Failure to create GLFWwindow.");
    }
}


/*
A WSI extension is needed to provide the connection between a platform's window system and the Vulkan API
Vk_KHR_surface is an abstract surface type for presenting rendered images
This must occur immediately after instance creation as a physical device needs to be selected based on whether it supports the created surface
*/
void Application::create_window_surface() {
    VkSurfaceKHR c_api_surface; // GLFW works with C API, so create surface for C API
    if (glfwCreateWindowSurface(*instance, window, nullptr, &c_api_surface)) { // Just using GLFW method for platform-agnosticism (see Vulkan tutorial on this part for details of what happens BTS)
        throw std::runtime_error("Failed to create window surface.");
    } // If this returns a zero-value, then creation of the window surface was successful
    surface = vk::raii::SurfaceKHR(instance, c_api_surface); // Now place this within the C++ wrapper for the surface
}
// After this we then need to pick the correct physical device that supports this surface, see additions to device.cpp