#include "application.hpp"

std::vector<const char*> Application::get_instance_extensions(u32& extension_count) {
    auto glfw_extensions = glfwGetRequiredInstanceExtensions(&extension_count);
    std::vector<const char*> extensions {glfw_extensions, glfw_extensions + extension_count};
   
    if (enable_validation_layers) {
        extensions.push_back(vk::EXTDebugUtilsExtensionName);
        extension_count++;
    }

    return extensions;;
}

std::vector<const char*> Application::load_extensions(u32& extension_count) {
        
    auto instance_extensions = get_instance_extensions(extension_count);
    std::vector<vk::ExtensionProperties> extension_properties = context.enumerateInstanceExtensionProperties();

    for (u32 i = 0; i < extension_count; i++) {
        const char* instance_extension = instance_extensions[i];
        std::cout << "Instance extension " << (int)i << ": " << instance_extension << "\n";
        
        bool vulkan_has_extension = std::ranges::any_of(
            extension_properties,
            [instance_extension](const vk::ExtensionProperties& vk_extension) {
                return std::string_view(instance_extension) == vk_extension.extensionName;
            }
        );

        if (!vulkan_has_extension) {
            throw std::runtime_error("Vulkan context lacks extension required by GLFW.");
        }
    }

    return instance_extensions;
}

std::vector<const char*> Application::load_layers(u32& layer_count) {
    
    std::vector<vk::LayerProperties> layer_properties = context.enumerateInstanceLayerProperties();
    layer_count = required_layers.size();

    for (u32 i = 0; i < layer_count; i++) {
        const char* required_layer = required_layers[i];
        std::cout << "Layer " << (int)i << ": " << required_layer << "\n";

        bool vulkan_has_layer = std::ranges::any_of(
            layer_properties,
            [required_layer](const vk::LayerProperties& vk_layer) {
                return std::string_view(required_layer) == vk_layer.layerName;
            }
        );

        if (!vulkan_has_layer) {
            throw std::runtime_error(std::string("Vulkan context lacks required layer: ") + required_layer);
        }
    }

    return required_layers;
}

void Application::create_instance() {

    constexpr vk::ApplicationInfo app_info = {
        .pApplicationName = "Vulkan Triangle",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "No Engine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = vk::ApiVersion14
    };

    u32 extension_count = 0;
    std::vector<const char*> extensions = load_extensions(extension_count);

    u32 layer_count = 0;
    std::vector<const char*> layers = enable_validation_layers ? load_layers(layer_count) : std::vector<const char*>{};

    vk::InstanceCreateInfo create_info = {
        .pApplicationInfo = &app_info,
        .enabledLayerCount = layer_count,
        .ppEnabledLayerNames = layers.data(),
        .enabledExtensionCount = extension_count,
        .ppEnabledExtensionNames = extensions.data()
    };

    instance = vk::raii::Instance(context, create_info);
}