#include "application.hpp"
#include <map>

bool Application::is_physical_device_suitable(const vk::raii::PhysicalDevice& device) {
	vk::PhysicalDeviceProperties properties = device.getProperties();
	vk::PhysicalDeviceFeatures features = device.getFeatures();

	bool supports_vulkan_13 = (properties.apiVersion >= vk::ApiVersion13);
	
	int qfp_index = 0;

	// Key addition to physical device selection has occurred here
	// Need to check that the selected queue family both supports graphics AND the surface that was just created
	bool supports_graphics = std::ranges::any_of(
		device.getQueueFamilyProperties(),
		[&qfp_index, device, this](const vk::QueueFamilyProperties& qfp) {
			bool supports_surface = device.getSurfaceSupportKHR(qfp_index++, *surface);
			return !!(qfp.queueFlags & vk::QueueFlagBits::eGraphics) && supports_surface;
		}
	);

	std::vector<const char*> required_extensions = {
		vk::KHRSwapchainExtensionName
	};

	std::vector<vk::ExtensionProperties> available_extensions = device.enumerateDeviceExtensionProperties();

	bool supports_required_extensions = std::ranges::all_of(
		required_extensions,
		[available_extensions](const auto& required_extension) {
			return std::ranges::any_of(
				available_extensions,
				[required_extension](const auto& available_extension) {
					return std::string_view(required_extension) == available_extension.extensionName;
				}
			);
		}
	);

	auto features2 = device.template getFeatures2<vk::PhysicalDeviceFeatures2,
												  vk::PhysicalDeviceVulkan11Features,
												  vk::PhysicalDeviceVulkan13Features,
												  vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();

	bool supports_required_features = features2.template get<vk::PhysicalDeviceVulkan11Features>().shaderDrawParameters &&
									  features2.template get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering &&
									  features2.template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState;

	return supports_vulkan_13 &&
		   supports_graphics &&
		   supports_required_extensions &&
		   supports_required_features;
}

int Application::get_physical_device_suitability_score(const vk::raii::PhysicalDevice& device) {
	int score = 0;

	vk::PhysicalDeviceProperties properties = device.getProperties();
	vk::PhysicalDeviceFeatures features = device.getFeatures();

	if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
		score += 1000;
	}

	score += properties.limits.maxImageDimension2D;

	if (!features.geometryShader) {
		score = 0;
	}

	return score;
}

void Application::pick_physical_device() {
	std::vector<vk::raii::PhysicalDevice> devices = instance.enumeratePhysicalDevices();

	if (devices.empty()) {
		throw std::runtime_error("No physical devices available to Vulkan.");
	}

	std::multimap<int, vk::raii::PhysicalDevice> candidates;

	for (auto device : devices) {
		if (!is_physical_device_suitable(device)) {
			continue;
		}

		int score = get_physical_device_suitability_score(device);
		candidates.insert(std::make_pair(score, device));
	}

	if (!candidates.empty() && candidates.rbegin()->first) {
		physical_device = candidates.rbegin()->second;
	} else {
		throw std::runtime_error("Physical devices are available to Vulkan, but none are suitable.");
	}
}

void Application::create_logical_device() {
	std::vector<vk::QueueFamilyProperties> queue_family_properties = physical_device.getQueueFamilyProperties();
	
	auto first_graphics_property = std::ranges::find_if(
		queue_family_properties,
		[](const vk::QueueFamilyProperties& qfp) {
			return !!(qfp.queueFlags & vk::QueueFlagBits::eGraphics);
		}
	);

	u32 first_graphics_property_index = std::distance(queue_family_properties.begin(), first_graphics_property);
	float graphics_property_priority = 0.5f;

	vk::DeviceQueueCreateInfo device_queue_create_info = {
		.queueFamilyIndex = first_graphics_property_index,
		.queueCount = 1,
		.pQueuePriorities = &graphics_property_priority
	};

	vk::PhysicalDeviceFeatures physical_device_features;

	vk::StructureChain<vk::PhysicalDeviceFeatures2,
					   vk::PhysicalDeviceVulkan11Features,
					   vk::PhysicalDeviceVulkan13Features,
					   vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT> feature_chain = {
					       { },
					       { .shaderDrawParameters = true },
					       { .dynamicRendering = true },
					       { .extendedDynamicState = true }
					   };

	std::vector<const char*> required_extensions = {
		vk::KHRSwapchainExtensionName
	};

	vk::DeviceCreateInfo device_create_info = {
		.pNext = &feature_chain.get<vk::PhysicalDeviceFeatures2>(),
		.queueCreateInfoCount = 1,
		.pQueueCreateInfos = &device_queue_create_info,
		.enabledExtensionCount = static_cast<u32>(required_extensions.size()),
		.ppEnabledExtensionNames = required_extensions.data()
	};

	logical_device = vk::raii::Device(physical_device, device_create_info);
	graphics_queue = vk::raii::Queue(logical_device, first_graphics_property_index, 0);
}