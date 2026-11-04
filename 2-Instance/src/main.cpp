// All code from this base is written from memory, using only notes I have written as a guide
// Comments are provided where additions occurred
// Areas with no comments were from prior sections

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <cstdint>
#include <algorithm>
#include <vector>
#include <string_view>

constexpr int WIDTH = 960;
constexpr int HEIGHT = 720;

using u32 = uint32_t;

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

    void create_instance() {

        // Create application info (required by create_info later)
        constexpr vk::ApplicationInfo app_info = {
            .pApplicationName = "Vulkan Triangle",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "No Engine",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = vk::ApiVersion14 // 1.4 is the baseline for the type of Vulkan I am learning
        };

        // Type signatures are kept for learning reasons, auto is preferred here for simplicity

        u32 glfw_extension_count = 0;
        const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count); // C-style array of const char*, hence const char**
        std::vector<vk::ExtensionProperties> extension_properties = context.enumerateInstanceExtensionProperties();

        // This iterates through all of the extension properties (the instance extensions supported by the Vulkan implementation)
        // And ensures that each extension required by GLFW is an extension that Vulkan has
        // Otherwise a runtime error stating that Vulkan does not support a required extension is given

        for (u32 i = 0; i < glfw_extension_count; i++) {
            const char* glfw_extension = glfw_extensions[i];
            
            // Using std::ranges::any_of()
            bool vulkan_has_extension = std::ranges::any_of(
                extension_properties,
                [glfw_extension](const vk::ExtensionProperties& vk_extension) {
                    return std::string_view(glfw_extension) == vk_extension.extensionName;
                }
            );
            // Syntax for lambdas in C++
            // [capture list](arg, specifically from iterator being iterated)
            // {
            //     return (condition here)
            // }
            // capture list = list of variables from outside the predicate function's scope

            // Useful to also know from std::ranges (follow same syntax): any_of, all_of, none_of, find_if, count_if (among many others)

            if (!vulkan_has_extension) { // Runtime error for if Vulkan lacks the current extension
                throw std::runtime_error("Vulkan context lacks extension required by GLFW.");
            }
        }

        // Can then use create_info, which takes in the application info provided earlier, and the properties
        vk::InstanceCreateInfo create_info = {
            .pApplicationInfo = &app_info, // app_info from earlier, takes vk::ApplicationInfo
            .enabledExtensionCount = glfw_extension_count, // takes u32
            .ppEnabledExtensionNames = glfw_extensions // takes const char**
        };

        instance = vk::raii::Instance(context, create_info); // Instance can now be created
    }

    void run() {
        initialise_window();
        create_instance(); // 1. Create instance function, see private:
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

private:
    vk::raii::Context context; // 2. Create context and instance
    vk::raii::Instance instance = nullptr;

    GLFWwindow* window = nullptr;

};

int main() {
    Application application;
    application.run();
}