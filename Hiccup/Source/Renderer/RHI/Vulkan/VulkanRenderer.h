// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/Core.h"
#include "VulkanDefines.h"

#include "Renderer/Renderer.h"

#include <vulkan/vulkan.h>

#define HC_ENABLE_VULKAN_VALIDATION 1

namespace HC
{

struct QueueFamilies
{
	bool has_graphics_queue;
	uint32 graphics_queue_index;

	bool has_present_queue;
	uint32 present_queue_index;

	bool has_compute_queue;
	uint32 compute_queue_index;

	bool has_transfer_queue;
	uint32 transfer_queue_index;
};

struct PhysicalDeviceCapabilities
{
	RendererCapabilities render_capabilities;
	VkPhysicalDeviceFeatures features;
};

class VulkanRenderer
{
public:
	static bool initialize(const RendererDescription& description);
	static void shutdown();

public:
	NODISCARD static VkInstance instance();
	NODISCARD static VkDevice device();
	NODISCARD static VkAllocationCallbacks* allocator();

public:
	// Wrapper around Renderer::get_next_render_context_index().
	ALWAYS_INLINE static uint32 get_next_render_context_index()
	{
		return Renderer::get_next_render_context_index();
	}

public:
	static RendererResult pick_swapchain_surface_format(VkSurfaceKHR surface_handle, VkSurfaceFormatKHR* out_format);
	static RendererResult pick_swapchain_present_mode(VkSurfaceKHR surface_handle, VkPresentModeKHR* out_present_mode);

private:
	static RendererResult create_instance();

	static bool check_instance_layers(
		Span<const char*>   required,
		Span<const char*>   optional,
		Array<const char*>& out_enabled
	);

	static bool check_instance_extensions(
		Span<const char*>   required,
		Span<const char*>   optional,
		Array<const char*>& out_enabled
	);

	static RendererResult create_debug_messenger();

	static RendererResult pick_physical_device();

	static uint64 evaluate_physical_device(VkPhysicalDevice physical_device, QueueFamilies* out_queue_families);

	static RendererResult create_device();

	static bool check_device_extensions(
		Span<const char*>   required,
		Span<const char*>   optional,
		Array<const char*>* out_enabled
	);

private:
	friend class Renderer;
};

} // namespace HC