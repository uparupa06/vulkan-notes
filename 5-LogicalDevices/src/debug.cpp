#include "application.hpp"

VKAPI_ATTR vk::Bool32 VKAPI_CALL Application::debug_callback(
    vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
    vk::DebugUtilsMessageTypeFlagsEXT type,
    const vk::DebugUtilsMessengerCallbackDataEXT* callback_data,
    void* user_data
) {

    switch (severity) {
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose:
            std::cout << "Severity: eVerbose, a diagnostic message from a Vulkan component.\n";
            break;
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo:
            std::cout << "Severity: eInfo, just a message giving some information.\n";
            break;
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning:
            std::cout << "Severity: eWarning, a Vulkan warning.\n";
            break;
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eError:
            std::cout << "Severity: eError, you are using Vulkan in an invalid way.\n";
            break;
        default:
            std::cout << "Severity: unknown severity, you have done something pretty weird. Oh no!\n";
            break;
    }

    switch (static_cast<vk::DebugUtilsMessageTypeFlagBitsEXT>(static_cast<VkDebugUtilsMessageTypeFlagsEXT>(type))) {
        case vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral:
            std::cout << "Message type: eGeneral, a general message from Vulkan.\n";
            break;
        case vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation:
            std::cout << "Message type: eValidation, you are possibly about to make a mistake or have made a specification violation.\n";
            break;
        case vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance:
            std::cout << "Message type: ePerformance, you are using Vulkan in a non-optimal way.\n";
            break;
        default:
            std::cout << "Message type: unknown, you have done something so bizarre that Vulkan cannot identify the message type.\n";
            break;
    }

    std::cout << "Callback data:\n";
    std::cout << "Message: " << callback_data->pMessage << "\n";

    return vk::False;
}

void Application::initialise_debug_messenger() {
    if (!enable_validation_layers) {
        return;
    }

    vk::DebugUtilsMessageSeverityFlagsEXT severity_flags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | 
                                                         vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo    | 
                                                         vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | 
                                                         vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
    vk::DebugUtilsMessageTypeFlagsEXT type_flags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral    |
                                                 vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                                                 vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance);
    vk::DebugUtilsMessengerCreateInfoEXT messenger_create_info = {
        .messageSeverity = severity_flags,
        .messageType = type_flags,
        .pfnUserCallback = &debug_callback
    };

    debug_messenger = instance.createDebugUtilsMessengerEXT(messenger_create_info);
}
