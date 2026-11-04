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
#include <iostream>

constexpr int WIDTH = 960;
constexpr int HEIGHT = 720;

// Useful macro when intending for something to differ between Debug/Release builds
#ifdef NDEBUG // non-Debug build...
constexpr bool enable_validation_layers = false;
#else // Debug build...
constexpr bool enable_validation_layers = true;
#endif

using u32 = uint32_t;

const std::vector<char const*> required_layers = {
    "VK_LAYER_KHRONOS_validation"
};

class Application {
public:

    // application.cpp
    Application();
    ~Application();
    void run();

    // window.cpp
    void initialise_window();

    // debug.cpp
    static VKAPI_ATTR vk::Bool32 VKAPI_CALL debug_callback(
        vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
        vk::DebugUtilsMessageTypeFlagsEXT type,
        const vk::DebugUtilsMessengerCallbackDataEXT* callback_data,
        void* user_data
    );
    void initialise_debug_messenger();

    // setup.cpp
    std::vector<const char*> get_instance_extensions(u32& extension_count);
    std::vector<const char*> load_extensions(u32& extension_count);
    std::vector<const char*> load_layers(u32& layer_count);
    void create_instance();

private:
    vk::raii::Context context;
    vk::raii::Instance instance = nullptr;
    vk::raii::DebugUtilsMessengerEXT debug_messenger = nullptr;
    
    GLFWwindow* window = nullptr;

};