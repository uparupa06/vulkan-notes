#include "application.hpp"
#include <cassert>
#include <cstdint>
#include <limits>
#include <algorithm>

vk::SurfaceFormatKHR Application::choose_surface_format(const std::vector<vk::SurfaceFormatKHR>& available_surface_formats) {
	const auto format = std::ranges::find_if(
		available_surface_formats,
		[](const vk::SurfaceFormatKHR& format) {
			return format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;
		}
	);

	return format != available_surface_formats.end() ? *format : available_surface_formats[0];
}

vk::PresentModeKHR Application::choose_present_mode(const std::vector<vk::PresentModeKHR>& available_present_modes) {
	assert(std::ranges::any_of(available_present_modes,
		[](const vk::PresentModeKHR& present_mode) {
			return present_mode == vk::PresentModeKHR::eFifo;
		}
	));
	return std::ranges::any_of(available_present_modes,
		[](const vk::PresentModeKHR& present_mode) {
			return present_mode == vk::PresentModeKHR::eMailbox;
		}
	) ? vk::PresentModeKHR::eMailbox : vk::PresentModeKHR::eFifo;
}

vk::Extent2D Application::choose_swap_extent(const vk::SurfaceCapabilitiesKHR& surface_capabilities) {
	if (surface_capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return surface_capabilities.currentExtent;
	}

	int w, h;
	glfwGetFramebufferSize(window, &w, &h);

	return {
		std::clamp<uint32_t>(w, surface_capabilities.minImageExtent.width,  surface_capabilities.maxImageExtent.width),
		std::clamp<uint32_t>(h, surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height)
	};
}

uint32_t Application::choose_min_image_count(const vk::SurfaceCapabilitiesKHR& surface_capabilities) {
	uint32_t min_image_count = std::max(3u, surface_capabilities.minImageCount);
	if (surface_capabilities.maxImageCount < min_image_count && surface_capabilities.maxImageCount > 0) {
		return surface_capabilities.maxImageCount;
	}
	return min_image_count;
}

void Application::build_swapchain() {
	vk::SurfaceCapabilitiesKHR surface_capabilities = physical_device.getSurfaceCapabilitiesKHR(*surface);
	std::vector<vk::SurfaceFormatKHR> available_surface_formats = physical_device.getSurfaceFormatsKHR(*surface);
	std::vector<vk::PresentModeKHR> available_present_modes = physical_device.getSurfacePresentModesKHR(*surface);

	swapchain_surface_format = choose_surface_format(available_surface_formats);
	swapchain_extent = choose_swap_extent(surface_capabilities);
	uint32_t min_image_count = choose_min_image_count(surface_capabilities);
	vk::PresentModeKHR present_mode = choose_present_mode(available_present_modes);

	vk::SwapchainCreateInfoKHR swapchain_create_info {
		.surface          = *surface,
		.minImageCount    = min_image_count,
		.imageFormat      = swapchain_surface_format.format,
		.imageColorSpace  = swapchain_surface_format.colorSpace,
		.imageExtent      = swapchain_extent,
		.imageArrayLayers = 1,
		.imageUsage       = vk::ImageUsageFlagBits::eColorAttachment,
		.imageSharingMode = vk::SharingMode::eExclusive,
		.preTransform     = surface_capabilities.currentTransform,
		.compositeAlpha   = vk::CompositeAlphaFlagBitsKHR::eOpaque,
		.presentMode      = present_mode,
		.clipped          = true,
		.oldSwapchain     = nullptr,
	};

	swapchain = vk::raii::SwapchainKHR(logical_device, swapchain_create_info);
	swapchain_images = swapchain.getImages();
}