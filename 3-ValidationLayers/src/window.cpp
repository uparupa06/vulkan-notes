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