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

#ifdef NDEBUG
constexpr bool enable_validation_layers = false;
#else
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
    void create_window_surface();

    // debug.cpp
    static VKAPI_ATTR vk::Bool32 VKAPI_CALL debug_callback(
        vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
        vk::DebugUtilsMessageTypeFlagsEXT type,
        const vk::DebugUtilsMessengerCallbackDataEXT* callback_data,
        void* user_data
    );
    void initialise_debug_messenger();

    // device.cpp
    bool is_physical_device_suitable(const vk::raii::PhysicalDevice& device);
    int get_physical_device_suitability_score(const vk::raii::PhysicalDevice& device);
    void pick_physical_device();
    void create_logical_device();

    // setup.cpp
    std::vector<const char*> get_instance_extensions(u32& extension_count);
    std::vector<const char*> load_extensions(u32& extension_count);
    std::vector<const char*> load_layers(u32& layer_count);
    void create_instance();

    // swapchain.cpp
    vk::SurfaceFormatKHR choose_surface_format(const std::vector<vk::SurfaceFormatKHR>& available_surface_formats);
    vk::PresentModeKHR choose_present_mode(const std::vector<vk::PresentModeKHR>& available_present_modes);
    vk::Extent2D choose_swap_extent(const vk::SurfaceCapabilitiesKHR& surface_capabilities);
    uint32_t choose_min_image_count(const vk::SurfaceCapabilitiesKHR& surface_capabilities);
    void build_swapchain();

private:
    vk::raii::Context context;
    vk::raii::Instance instance = nullptr;
    vk::raii::DebugUtilsMessengerEXT debug_messenger = nullptr;
    vk::raii::PhysicalDevice physical_device = nullptr;
    vk::raii::Device logical_device = nullptr;
    vk::raii::Queue graphics_queue = nullptr;
    vk::raii::SurfaceKHR surface = nullptr; // SurfaceKHR member
    
    vk::raii::SwapchainKHR swapchain = nullptr;
    std::vector<vk::Image> swapchain_images;
    vk::SurfaceFormatKHR swapchain_surface_format;
    vk::Extent2D swapchain_extent;

    GLFWwindow* window = nullptr;

};