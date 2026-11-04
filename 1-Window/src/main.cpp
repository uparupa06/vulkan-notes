// All code from this base is written from memory, using only notes I have written as a guide

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <cstdlib>

constexpr int WIDTH = 960;
constexpr int HEIGHT = 720;

class Application {
public:

    Application() {}

    ~Application() {
        if (window) {
            glfwDestroyWindow(window);
        }
        glfwTerminate();
    }

    void initialise_window() {
        if (!glfwInit()) {
            throw std::runtime_error("Failure to initialise GLFW.");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // because we are using Vulkan, tell GLFW that it is not being used for OpenGL
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

        if (!window) {
            throw std::runtime_error("Failure to create GLFWwindow.");
        }
    }

    void run() {
        initialise_window();
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

private:
    GLFWwindow* window = nullptr;
};

int main() {
    Application application;
    application.run();
}