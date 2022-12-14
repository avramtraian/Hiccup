// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "VulkanRenderer.h"

#include "Core/Application.h"

#include "VulkanRenderContext.h"
#include "VulkanSwapchain.h"

#if HC_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <vulkan/vulkan_win32.h>
#elif HC_PLATFORM_LINUX
	#include <vulkan/vulkan_xcb.h>
#elif HC_PLATFORM_MACOS
	#include <vulkan/vulkan_macos.h>
#endif // Platform switch.

namespace HC
{

struct VulkanRendererData
{
	VkAllocationCallbacks* allocator = nullptr;

	VkInstance instance = VK_NULL_HANDLE;

	VkDebugUtilsMessengerEXT debug_messenger = VK_NULL_HANDLE;

	VkPhysicalDevice physical_device = VK_NULL_HANDLE;

	QueueFamilies queue_families;

	PhysicalDeviceCapabilities physical_device_capabilities;

	VkDevice device = VK_NULL_HANDLE;

	VkQueue graphics_queue = VK_NULL_HANDLE;
	VkQueue present_queue = VK_NULL_HANDLE;
	VkQueue compute_queue = VK_NULL_HANDLE;
	VkQueue transfer_queue = VK_NULL_HANDLE;

	uint32 primary_render_context_index = 0xFFFFFFFF;
};
static_internal VulkanRendererData* s_vulkan_data = nullptr;

namespace Utils
{

static_internal void log_instance_layers(Span<const char*> required, Span<const char*> optional)
{
	HC_LOG_TRACE_TAG("VULKAN", "Searching for Vulkan instance layers:");

	HC_LOG_TRACE_TAG("VULKAN", "    Required:");
	for (usize i = 0; i < required.count(); ++i)
	{
		HC_LOG_TRACE_TAG("VULKAN", "        - %s", required.values()[i]);
	}

	HC_LOG_TRACE_TAG("VULKAN", "    Optional:");
	for (usize i = 0; i < optional.count(); ++i)
	{
		HC_LOG_TRACE_TAG("VULKAN", "        - %s", optional.values()[i]);
	}
}

static_internal void log_instance_extensions(Span<const char*> required, Span<const char*> optional)
{
	HC_LOG_TRACE_TAG("VULKAN", "Searching for Vulkan instance extensions:");

	HC_LOG_TRACE_TAG("VULKAN", "    Required:");
	for (usize i = 0; i < required.count(); ++i)
	{
		HC_LOG_TRACE_TAG("VULKAN", "        - %s", required.values()[i]);
	}

	HC_LOG_TRACE_TAG("VULKAN", "    Optional:");
	for (usize i = 0; i < optional.count(); ++i)
	{
		HC_LOG_TRACE_TAG("VULKAN", "        - %s", optional.values()[i]);
	}
}

static_internal void log_device_extensions(Span<const char*> required, Span<const char*> optional)
{
	HC_LOG_TRACE_TAG("VULKAN", "Searching for Vulkan device extensions:");

	HC_LOG_TRACE_TAG("VULKAN", "    Required:");
	for (usize i = 0; i < required.count(); ++i) {
		HC_LOG_TRACE_TAG("VULKAN", "        - %s", required.values()[i]);
	}

	HC_LOG_TRACE_TAG("VULKAN", "    Optional:");
	for (usize i = 0; i < optional.count(); ++i) {
		HC_LOG_TRACE_TAG("VULKAN", "        - %s", optional.values()[i]);
	}
}

} // namespace HC::Utils

static_internal VkBool32 vulkan_debug_callback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
	{
		HC_LOG_ERROR_TAG("VULKAN", "Validation layer error: %s\n", pCallbackData->pMessage);
	}
	else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
	{
		HC_LOG_WARN_TAG("VULKAN", "Validation layer warning: %s\n", pCallbackData->pMessage);
	}

	return VK_FALSE;
}

bool VulkanRenderer::initialize(const RendererDescription& description)
{
	s_vulkan_data = hc_new VulkanRendererData();

	if (auto r = create_instance(); r != RendererResult::success)
	{
		HC_LOG_ERROR_TAG("VULKAN", "Failed to create the Vulkan instance! Code: %s", renderer_result_to_string(r));
		return false;
	}

	if (auto r = create_debug_messenger(); r != RendererResult::success)
	{
		HC_LOG_ERROR_TAG("VULKAN", "Failed to create the Vulkan debug messenger! Code: %s", renderer_result_to_string(r));
		return false;
	}

	if (!Renderer::create_render_contexts())
	{
		// The error is logged during enderer::create_render_contexts().
		return false;
	}
	if (s_vulkan_data->primary_render_context_index == 0xFFFFFFFF)
	{
		HC_LOG_ERROR_TAG("RENDERER", "A single RenderingContext must be marked as primary. None were marked.");
		return false;
	}

	if (auto r = pick_physical_device(); r != RendererResult::success)
	{
		HC_LOG_ERROR_TAG("VULKAN", "Failed to pick the Vulkan physical device! Code: %s", renderer_result_to_string(r));
		return false;
	}

	if (auto r = create_device(); r != RendererResult::success)
	{
		HC_LOG_ERROR_TAG("VULKAN", "Failed to create the Vulkan logical device! Code: %s", renderer_result_to_string(r));
		return false;
	}

	if (!Renderer::create_swapchains())
	{
		// The error is logged during enderer::create_swapchains().
		return false;
	}

	return true;
}

void VulkanRenderer::shutdown()
{
	HC_LOG_TRACE_TAG("RENDERER", "Destroying the Vulkan logical device...");
	vkDestroyDevice(s_vulkan_data->device, s_vulkan_data->allocator);
	s_vulkan_data->device = VK_NULL_HANDLE;
	HC_LOG_INFO_TAG("RENDERER", "Destroyed the Vulkan logical device.");

	HC_LOG_TRACE_TAG("RENDERER", "Destroying the Vulkan debug messenger...");
	PFN_vkDestroyDebugUtilsMessengerEXT destroy_debug_utils_messenger = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance(), "vkDestroyDebugUtilsMessengerEXT");
	if (destroy_debug_utils_messenger)
	{
		destroy_debug_utils_messenger(instance(), s_vulkan_data->debug_messenger, s_vulkan_data->allocator);
	}
	else
	{
		HC_LOG_ERROR_TAG("RENDERER", "Failed to obtain the vkDestroyDebugReportCallbackEXT function pointer!");
	}
	HC_LOG_INFO_TAG("RENDERER", "Destroyed the Vulkan debug messenger.");

	HC_LOG_TRACE_TAG("RENDERER", "Destroying the Vulkan instance...");
	vkDestroyInstance(s_vulkan_data->instance, s_vulkan_data->allocator);
	s_vulkan_data->instance = VK_NULL_HANDLE;
	HC_LOG_INFO_TAG("RENDERER", "Destroyed the Vulkan instance.");

	hc_delete s_vulkan_data;
	s_vulkan_data = nullptr;
}

VkInstance VulkanRenderer::instance()
{
	return s_vulkan_data->instance;
}

VkDevice VulkanRenderer::device()
{
	return s_vulkan_data->device;
}

VkAllocationCallbacks* VulkanRenderer::allocator()
{
	return s_vulkan_data->allocator;
}

RendererResult VulkanRenderer::create_instance()
{
	VkApplicationInfo application_info = {};
	application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	application_info.apiVersion = VK_API_VERSION_1_3;
	application_info.pApplicationName = "HiccupApplication";
	application_info.applicationVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
	application_info.pEngineName = "Hiccup";
	application_info.engineVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);

	VkInstanceCreateInfo instance_info = {};
	instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_info.pApplicationInfo = &application_info;

	Array<const char*> required_layers;
	Array<const char*> optional_layers;
	Array<const char*> enabled_layers;

#if HC_ENABLE_VULKAN_VALIDATION
	optional_layers.add("VK_LAYER_KHRONOS_validation");
#endif // HC_ENABLE_VULKAN_VALIDATION

	Utils::log_instance_layers(required_layers.span(), optional_layers.span());

	if (!check_instance_layers(required_layers.span(), optional_layers.span(), enabled_layers))
	{
		return RendererResult::extension_missing;
	}

	Array<const char*> required_extensions;
	Array<const char*> optional_extensions;
	Array<const char*> enabled_extensions;

#if HC_PLATFORM_WINDOWS
	required_extensions.add("VK_KHR_surface");
	required_extensions.add("VK_KHR_win32_surface");
#endif // Platform switch.

#if HC_ENABLE_VULKAN_VALIDATION
	optional_extensions.add("VK_EXT_debug_utils");
#endif // HC_ENABLE_VULKAN_VALIDATION

	Utils::log_instance_extensions(required_extensions.span(), optional_extensions.span());

	if (!check_instance_extensions(required_extensions.span(), optional_extensions.span(), enabled_extensions))
	{
		return RendererResult::extension_missing;
	}

	instance_info.enabledLayerCount = (uint32)enabled_layers.size();
	instance_info.ppEnabledLayerNames = enabled_layers.data();
	instance_info.enabledExtensionCount = (uint32)enabled_extensions.size();
	instance_info.ppEnabledExtensionNames = enabled_extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debug_messenger_info = {};
	debug_messenger_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debug_messenger_info.pfnUserCallback = vulkan_debug_callback;
	debug_messenger_info.messageSeverity =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
	debug_messenger_info.messageType =
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	instance_info.pNext = &debug_messenger_info;

	if (vkCreateInstance(&instance_info, s_vulkan_data->allocator, &s_vulkan_data->instance) != VK_SUCCESS)
	{
		return RendererResult::rhi_resource_creation_failed;
	}

	return RendererResult::success;
}

bool VulkanRenderer::check_instance_layers(Span<const char*> required, Span<const char*> optional, Array<const char*>& out_enabled)
{
	out_enabled.clear();

	if (required.is_empty() && optional.is_empty())
	{
		return true;
	}

	out_enabled.set_capacity(required.count() + optional.count());

	uint32 available_layers_count;
	vkEnumerateInstanceLayerProperties(&available_layers_count, nullptr);

	Array<VkLayerProperties> available_layers;
	available_layers.set_size_uninitialized(available_layers_count);
	vkEnumerateInstanceLayerProperties(&available_layers_count, available_layers.data());

	bool found_all_required_layers = true;
	for (usize required_index = 0; required_index < required.count(); ++required_index)
	{
		bool found_layer = false;

		for (usize available_index = 0; available_index < available_layers.size(); ++available_index)
		{
			if (utf8_string_equals(required.values()[required_index], available_layers[available_index].layerName))
			{
				found_layer = true;
				break;
			}
		}

		if (found_layer)
		{
			out_enabled.add(required.values()[required_index]);
		}
		else
		{
			HC_LOG_ERROR_TAG("VULKAN", "Vulkan required instance layer '%s' not found!", required.values()[required_index]);
			found_all_required_layers = false;
		}
	}

	uint32 optional_layers_found = 0;
	for (usize optional_index = 0; optional_index < optional.count(); ++optional_index)
	{
		bool found_layer = false;

		for (usize available_index = 0; available_index < available_layers.size(); ++available_index)
		{
			if (utf8_string_equals(optional.values()[optional_index], available_layers[available_index].layerName))
			{
				found_layer = true;
				break;
			}
		}

		if (found_layer)
		{
			out_enabled.add(optional.values()[optional_index]);
			optional_layers_found++;
		}
		else
		{
			HC_LOG_WARN_TAG("VULKAN", "Vulkan optional instance layer '%s' not found!", optional.values()[optional_index]);
		}
	}

	if (found_all_required_layers)
	{
		HC_LOG_INFO_TAG("VULKAN", "All required Vulkan instance layers were found (%u layers).", required.count());
	}
	else
	{
		HC_LOG_ERROR_TAG("VULKAN", "Required Vulkan instance layers are missing! %u/%u found.", out_enabled.size() - optional_layers_found, required.count());
	}

	if (optional_layers_found == optional.count())
	{
		HC_LOG_INFO_TAG("VULKAN", "All optional Vulkan instance layers were found (%u layers).", optional.count());
	}
	else
	{
		HC_LOG_WARN_TAG("VULKAN", "Optinal Vulkan instance layers are missing! %u/%u found.", optional_layers_found, optional.count());
	}

	return found_all_required_layers;
}

bool VulkanRenderer::check_instance_extensions(Span<const char*> required, Span<const char*> optional, Array<const char*>& out_enabled)
{
	out_enabled.clear();

	if (required.is_empty() && optional.is_empty())
	{
		return true;
	}

	out_enabled.set_capacity(required.count() + optional.count());

	uint32 available_extensions_count;
	vkEnumerateInstanceExtensionProperties(nullptr, &available_extensions_count, nullptr);

	Array<VkExtensionProperties> available_extensions;
	available_extensions.set_size_uninitialized(available_extensions_count);
	vkEnumerateInstanceExtensionProperties(nullptr, &available_extensions_count, available_extensions.data());

	bool found_all_required_extensions = true;
	for (usize required_index = 0; required_index < required.count(); ++required_index)
	{
		bool found_extension = false;

		for (usize available_index = 0; available_index < available_extensions.size(); ++available_index)
		{
			if (utf8_string_equals(required.values()[required_index], available_extensions[available_index].extensionName))
			{
				found_extension = true;
				break;
			}
		}

		if (found_extension)
		{
			out_enabled.add(required.values()[required_index]);
		}
		else
		{
			HC_LOG_ERROR_TAG("VULKAN", "Vulkan required instance extension '%s' not found!", required.values()[required_index]);
			found_all_required_extensions = false;
		}
	}

	uint32 optional_extensions_found = 0;
	for (usize optional_index = 0; optional_index < optional.count(); ++optional_index)
	{
		bool found_extension = false;

		for (usize available_index = 0; available_index < available_extensions.size(); ++available_index)
		{
			if (utf8_string_equals(optional.values()[optional_index], available_extensions[available_index].extensionName))
			{
				found_extension = true;
				break;
			}
		}

		if (found_extension)
		{
			out_enabled.add(optional.values()[optional_index]);
			optional_extensions_found++;
		}
		else
		{
			HC_LOG_WARN_TAG("VULKAN", "Vulkan optional instance extension '%s' not found!", optional.values()[optional_index]);
		}
	}

	if (found_all_required_extensions)
	{
		HC_LOG_INFO_TAG("VULKAN", "All required Vulkan instance extensions were found (%u extensions).", required.count());
	}
	else
	{
		HC_LOG_ERROR_TAG("VULKAN", "Required Vulkan instance extensions are missing! %u/%u found.", out_enabled.size() - optional_extensions_found, required.count());
	}

	if (optional_extensions_found == optional.count())
	{
		HC_LOG_INFO_TAG("VULKAN", "All optional Vulkan instance extensions were found (%u extensions).", optional.count());
	}
	else
	{
		HC_LOG_WARN_TAG("VULKAN", "Optional Vulkan instance extensions are missing! %u/%u found.", optional_extensions_found, optional.count());
	}

	return found_all_required_extensions;
}

RendererResult VulkanRenderer::create_debug_messenger()
{
	VkDebugUtilsMessengerCreateInfoEXT debug_messenger_info = {};
	debug_messenger_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debug_messenger_info.pfnUserCallback = vulkan_debug_callback;
	debug_messenger_info.messageSeverity =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
	debug_messenger_info.messageType =
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	PFN_vkCreateDebugUtilsMessengerEXT create_debug_utils_messenger = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance(), "vkCreateDebugUtilsMessengerEXT");
	if (!create_debug_utils_messenger)
	{
		HC_LOG_ERROR_TAG("VULKAN", "Failed to obtain the vkCreateDebugUtilsMessengerEXT function pointer!");
		return RendererResult::rhi_resource_creation_failed;
	}

	if (create_debug_utils_messenger(instance(), &debug_messenger_info, s_vulkan_data->allocator, &s_vulkan_data->debug_messenger) != VK_SUCCESS)
	{
		return RendererResult::rhi_resource_creation_failed;
	}

	return RendererResult::success;
}

RendererResult VulkanRenderer::pick_physical_device()
{
	uint32 physical_devices_count = 0;
	HC_VULKAN_CHECK(vkEnumeratePhysicalDevices(s_vulkan_data->instance, &physical_devices_count, nullptr));

	Array<VkPhysicalDevice> physical_devices;
	physical_devices.set_size_uninitialized(physical_devices_count);
	HC_VULKAN_CHECK(vkEnumeratePhysicalDevices(s_vulkan_data->instance, &physical_devices_count, physical_devices.data()));

	Array<uint64> physical_device_scores;
    physical_device_scores.set_size_zeroed(physical_devices.size());
	Array<QueueFamilies> physical_devices_queue_families;
	physical_devices_queue_families.set_size_zeroed(physical_devices.size());

    for (usize i = 0; i < physical_devices.size(); ++i)
	{
        VkPhysicalDevice physical_device = physical_devices[i];
		physical_device_scores[i] = evaluate_physical_device(physical_device, &physical_devices_queue_families[i]);
	}

	usize max_score = 0;
	usize best_physical_device_index;
	for (usize i = 0; i < physical_device_scores.size(); ++i)
	{
		if (physical_device_scores[i] > max_score)
		{
			max_score = physical_device_scores[i];
			best_physical_device_index = i;
		}
	}

	if (max_score > 0)
	{
		VkPhysicalDevice physical_device = physical_devices[best_physical_device_index];
		s_vulkan_data->physical_device = physical_device;
		s_vulkan_data->queue_families = physical_devices_queue_families[best_physical_device_index];

		vkGetPhysicalDeviceFeatures(physical_device, &s_vulkan_data->physical_device_capabilities.features);

		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(physical_device, &properties);
		HC_LOG_INFO("Used GPU: %s.", properties.deviceName);
	}
	else
	{
		HC_LOG_ERROR_TAG("VULKAN", "No GPU has support for all the required Vulkan features!");
		return RendererResult::no_suitable_gpu;
	}

	return RendererResult::success;
}

uint64 VulkanRenderer::evaluate_physical_device(VkPhysicalDevice physical_device, QueueFamilies* out_queue_families)
{
	// TODO(Traian): Load from config file.
	constexpr uint64 Weight_IsDiscrete = 5;

	uint64 device_score = 0;

	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(physical_device, &properties);

	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(physical_device, &features);

	if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	{
		device_score += Weight_IsDiscrete;
	}

	uint32 queue_families_count;
	vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_families_count, nullptr);

	Array<VkQueueFamilyProperties> queue_families;
	queue_families.set_size_uninitialized(queue_families_count);
	vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_families_count, queue_families.data());

	*out_queue_families = {};

	const VulkanRenderContext* primary_render_context =
		(const VulkanRenderContext*)Renderer::get_render_context(s_vulkan_data->primary_render_context_index);

	for (usize i = 0; i < queue_families.size(); ++i) {
		const VkQueueFamilyProperties& queue_family = queue_families[i];

		if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT && !out_queue_families->has_graphics_queue) {
			out_queue_families->has_graphics_queue = true;
			out_queue_families->graphics_queue_index = (uint32)i;
		}
		if (queue_family.queueFlags & VK_QUEUE_COMPUTE_BIT && !out_queue_families->has_compute_queue) {
			out_queue_families->has_compute_queue = true;
			out_queue_families->compute_queue_index = (uint32)i;
		}
		if (queue_family.queueFlags & VK_QUEUE_TRANSFER_BIT && !out_queue_families->has_transfer_queue) {
			out_queue_families->has_transfer_queue = true;
			out_queue_families->transfer_queue_index = (uint32)i;
		}

		if (!out_queue_families->present_queue_index) {
			VkBool32 supports_presenting = VK_FALSE;
			HC_VULKAN_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, (uint32)i, primary_render_context->m_surface_handle, &supports_presenting));
			if (supports_presenting) {
				out_queue_families->has_present_queue = true;
				out_queue_families->present_queue_index = (uint32)i;
			}
		}
	}

	if (!out_queue_families->has_graphics_queue)
	{
		return 0;
	}

	return device_score;
}

namespace Utils
{

static_internal void add_queue_family(uint32 queue_family_index, uint32* queue_family_indices, uint32& queue_family_indices_count)
{
	queue_family_indices[queue_family_indices_count] = queue_family_index;

	bool already_exists = false;
	for (uint32 i = 0; i < queue_family_indices_count; ++i) {
		if (queue_family_indices[i] == queue_family_index) {
			already_exists = true;
			break;
		}
	}

	if (!already_exists) {
		queue_family_indices_count++;
	}
}

} // namespace HC::Utils

RendererResult VulkanRenderer::create_device()
{
	VkDeviceCreateInfo device_info = {};
	device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_info.pEnabledFeatures = &s_vulkan_data->physical_device_capabilities.features;
	
	Array<const char*> device_required_extensions;
	Array<const char*> device_optional_extensions;
	Array<const char*> device_enabled_extensions;

	device_required_extensions.add(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	Utils::log_device_extensions(device_required_extensions.span(), device_optional_extensions.span());

	if (!check_device_extensions(device_required_extensions.span(), device_optional_extensions.span(), &device_enabled_extensions))
	{
		return RendererResult::extension_missing;
	}
	
	device_info.enabledExtensionCount = (uint32)device_enabled_extensions.size();
	device_info.ppEnabledExtensionNames = device_enabled_extensions.data();
	
	uint32 queue_family_indices[4] = {};
	uint32 queue_family_indices_count = 0;

	Utils::add_queue_family(s_vulkan_data->queue_families.graphics_queue_index, queue_family_indices, queue_family_indices_count);

	if (s_vulkan_data->queue_families.has_compute_queue) {
		Utils::add_queue_family(s_vulkan_data->queue_families.compute_queue_index, queue_family_indices, queue_family_indices_count);
	}

	if (s_vulkan_data->queue_families.has_transfer_queue) {
		Utils::add_queue_family(s_vulkan_data->queue_families.transfer_queue_index, queue_family_indices, queue_family_indices_count);
	}

	if (s_vulkan_data->queue_families.has_present_queue) {
		Utils::add_queue_family(s_vulkan_data->queue_families.present_queue_index, queue_family_indices, queue_family_indices_count);
	}

	VkDeviceQueueCreateInfo queues_infos[4] = {};

	for (uint32 i = 0; i < queue_family_indices_count; ++i) {
		VkDeviceQueueCreateInfo& queue_info = queues_infos[i];

		queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_info.queueFamilyIndex = queue_family_indices[i];
		const float priority = 1.0F;
		queue_info.queueCount = 1;
		queue_info.pQueuePriorities = &priority;
	}

	device_info.queueCreateInfoCount = queue_family_indices_count;
	device_info.pQueueCreateInfos = queues_infos;

	if (vkCreateDevice(s_vulkan_data->physical_device, &device_info, s_vulkan_data->allocator, &s_vulkan_data->device) != VK_SUCCESS) {
		HC_LOG_ERROR_TAG("VULKAN", "Failed to create the Vulkan logical device!");
		return RendererResult::rhi_resource_creation_failed;
	}

	vkGetDeviceQueue(s_vulkan_data->device, s_vulkan_data->queue_families.graphics_queue_index, 0, &s_vulkan_data->graphics_queue);
	
	if (s_vulkan_data->queue_families.has_compute_queue) {
		vkGetDeviceQueue(s_vulkan_data->device, s_vulkan_data->queue_families.compute_queue_index, 0, &s_vulkan_data->compute_queue);
	}
	
	if (s_vulkan_data->queue_families.has_transfer_queue) {
		vkGetDeviceQueue(s_vulkan_data->device, s_vulkan_data->queue_families.transfer_queue_index, 0, &s_vulkan_data->transfer_queue);
	}

	if (s_vulkan_data->queue_families.has_present_queue) {
		vkGetDeviceQueue(s_vulkan_data->device, s_vulkan_data->queue_families.present_queue_index, 0, &s_vulkan_data->present_queue);
	}

	return RendererResult::success;
}

bool VulkanRenderer::check_device_extensions(Span<const char*> required, Span<const char*> optional, Array<const char*>* out_enabled)
{
	out_enabled->clear();

	if (required.is_empty() && optional.is_empty()) {
		return true;
	}

	out_enabled->set_capacity(required.count() + optional.count());

	uint32 available_extensions_count = 0;
	HC_VULKAN_CHECK(vkEnumerateDeviceExtensionProperties(s_vulkan_data->physical_device, nullptr, &available_extensions_count, nullptr));

	Array<VkExtensionProperties> available_extensions;
	available_extensions.set_size_uninitialized(available_extensions_count);
	HC_VULKAN_CHECK(vkEnumerateDeviceExtensionProperties(s_vulkan_data->physical_device, nullptr, &available_extensions_count, available_extensions.data()));

	bool found_all_required_extensions = true;
	for (usize required_index = 0; required_index < required.count(); ++required_index) {
		const char* required_extension = required.values()[required_index];

		bool extension_found = false;
		for (usize available_index = 0; available_index < available_extensions.size(); ++available_index) {
			if (utf8_string_equals(required_extension, available_extensions[available_index].extensionName))
			{
				extension_found = true;
				break;
			}
		}

		if (extension_found) {
			out_enabled->add(required_extension);
		}
		else {
			HC_LOG_ERROR_TAG("VULKAN", "Vulkan required device extension '%s' not found!", required_extension);
			found_all_required_extensions = false;
		}
	}

	uint32 optional_extensions_found = 0;
	for (usize optional_index = 0; optional_index < optional.count(); ++optional_index) {
		const char* optional_extension = optional.values()[optional_index];
		
		bool found_extension = false;
		for (usize available_index = 0; available_index < available_extensions.size(); ++available_index) {
			if (utf8_string_equals(optional_extension, available_extensions[available_index].extensionName))
			{
				found_extension = true;
				break;
			}
		}

		if (found_extension) {
			out_enabled->add(optional_extension);
			optional_extensions_found++;
		}
		else {
			HC_LOG_WARN_TAG("VULKAN", "Vulkan optional device extension '%s' not found!", optional_extension);
		}
	}

	if (found_all_required_extensions) {
		HC_LOG_INFO_TAG("VULKAN", "All required Vulkan device extensions were found (%u extensions).", required.count());
	}
	else {
		HC_LOG_ERROR_TAG("VULKAN", "Required Vulkan device extensions are missing! %u/%u found.", out_enabled->size() - optional_extensions_found, required.count());
	}

	if (optional_extensions_found == optional.count()) {
		HC_LOG_INFO_TAG("VULKAN", "All optional Vulkan device extensions were found (%u extensions).", optional.count());
	}
	else {
		HC_LOG_WARN_TAG("VULKAN", "Optional Vulkan device extensions are missing! %u/%u found.", optional_extensions_found, optional.count());
	}

	return found_all_required_extensions;
}

VulkanRenderContext::VulkanRenderContext(const RenderContextDescription& description, RendererResult* out_result)
	: m_surface_handle(VK_NULL_HANDLE)
	, m_owner(nullptr)
{
	*out_result = RendererResult::success;

	m_owner = description.owner;
	if (!m_owner) {
		*out_result = RendererResult::invalid_description;
		HC_LOG_ERROR_TAG("VULKAN", "RenderContextDescription::owner must be a valid pointer to the context's owner window! nullptr was provided.");
		return;
	}

#if HC_PLATFORM_WINDOWS

	VkWin32SurfaceCreateInfoKHR surface_info = {};
	surface_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surface_info.hinstance = GetModuleHandle(NULL);
	surface_info.hwnd = (HWND)description.owner->get_native_handle();

	if (vkCreateWin32SurfaceKHR(s_vulkan_data->instance, &surface_info, s_vulkan_data->allocator, &m_surface_handle) != VK_SUCCESS) {
		*out_result = RendererResult::rhi_resource_creation_failed;
		return;
	}

#endif // Platform switch.

	if (description.is_primary_context) {
		s_vulkan_data->primary_render_context_index = VulkanRenderer::get_next_render_context_index();
	}
}

VulkanRenderContext::~VulkanRenderContext()
{
	m_swapchain.release();

	vkDestroySurfaceKHR(s_vulkan_data->instance, m_surface_handle, s_vulkan_data->allocator);
	m_surface_handle = VK_NULL_HANDLE;
}

RendererResult VulkanRenderContext::create_swapchain()
{
	SwapchainDescription swapchain_desc = {};
	swapchain_desc.owning_context = this;

	UniquePtr<Swapchain> swapchain;
	RendererResult result = Swapchain::create(swapchain_desc, &swapchain);
	m_swapchain = swapchain.as<VulkanSwapchain>();
	return result;
}

VulkanSwapchain::VulkanSwapchain(const SwapchainDescription& description, RendererResult* out_result)
	: m_swapchain_handle(VK_NULL_HANDLE)
	, m_owning_context(nullptr)

	, m_min_image_count(0)
	, m_surface_format{}
	, m_image_sharing_mode{}
	, m_queue_family_indices_count(0)
	, m_queue_family_indices{}
	, m_present_mode{}
	, m_pre_transform{}
{
	*out_result = RendererResult::success;

	m_owning_context = (VulkanRenderContext*)description.owning_context;
	if (!m_owning_context) {
		*out_result = RendererResult::invalid_description;
		HC_LOG_ERROR_TAG("VULKAN", "SwapchainDescription::owning_context must be a valid pointer to the swapchain's owner! nullptr was provided.");
		return;
	}

	if (auto r = query_parameters(); r != RendererResult::success) {
		*out_result = r;
		HC_LOG_ERROR_TAG("VULKAN", "Failed to query the Vulkan swapchain parameters!");
		return;
	}

	if (auto r = invalidate(); r != RendererResult::success) {
		*out_result = r;
		return;
	}
}

VulkanSwapchain::~VulkanSwapchain()
{
	for (usize i = 0; i < m_image_views.size(); ++i) {
		vkDestroyImageView(s_vulkan_data->device, m_image_views[i], s_vulkan_data->allocator);
	}

	vkDestroySwapchainKHR(s_vulkan_data->device, m_swapchain_handle, s_vulkan_data->allocator);
	m_swapchain_handle = VK_NULL_HANDLE;
}

RendererResult VulkanSwapchain::query_parameters()
{
	if (auto r = VulkanRenderer::pick_swapchain_surface_format(m_owning_context->m_surface_handle, &m_surface_format); r != RendererResult::success) {
		HC_LOG_ERROR_TAG("VULKAN", "No physical device surface format matches the requirements!");
		return r;
	}

	if (auto r = VulkanRenderer::pick_swapchain_present_mode(m_owning_context->m_surface_handle, &m_present_mode); r != RendererResult::success) {
		HC_LOG_ERROR_TAG("VULKAN", "No physical device present mode matches the requirements!");
		return r;
	}

	HC_VULKAN_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(s_vulkan_data->physical_device, m_owning_context->m_surface_handle, &m_surface_caps));

	m_min_image_count = m_surface_caps.minImageCount + 1;
	if (m_surface_caps.maxImageCount > 0 && m_min_image_count > m_surface_caps.maxImageCount) {
		m_min_image_count = m_surface_caps.maxImageCount;
	}

	const uint32 present_index = s_vulkan_data->queue_families.present_queue_index;
	const uint32 graphics_index = s_vulkan_data->queue_families.graphics_queue_index;

	if (graphics_index == present_index) {
		m_image_sharing_mode = VK_SHARING_MODE_CONCURRENT;
		m_queue_family_indices_count = 2;
		m_queue_family_indices[0] = graphics_index;
		m_queue_family_indices[1] = present_index;
	}
	else {
		m_image_sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
		m_queue_family_indices_count = 0;
	}

	return RendererResult::success;
}

RendererResult VulkanSwapchain::invalidate()
{
	VkExtent2D swapchain_extent = {};

	if (m_surface_caps.currentExtent.width == 0xFFFFFFFF && m_surface_caps.currentExtent.height == 0xFFFFFFFF) {
		const uint32 window_width = m_owning_context->m_owner->get_width();
		const uint32 window_height = m_owning_context->m_owner->get_height();
		swapchain_extent.width = Math::clamp(window_width, m_surface_caps.minImageExtent.width, m_surface_caps.maxImageExtent.width);
		swapchain_extent.height = Math::clamp(window_height, m_surface_caps.minImageExtent.height, m_surface_caps.maxImageExtent.height);
	}
	else {
		swapchain_extent = m_surface_caps.currentExtent;
	}

	VkSwapchainCreateInfoKHR swapchain_info = {};
	swapchain_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchain_info.surface = m_owning_context->m_surface_handle;

	swapchain_info.minImageCount = m_min_image_count;
	swapchain_info.imageFormat = m_surface_format.format;
	swapchain_info.imageColorSpace = m_surface_format.colorSpace;
	swapchain_info.imageExtent = swapchain_extent;
	swapchain_info.imageArrayLayers = 1;
	swapchain_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	swapchain_info.queueFamilyIndexCount = m_queue_family_indices_count;
	swapchain_info.pQueueFamilyIndices = m_queue_family_indices;

	swapchain_info.preTransform = m_surface_caps.currentTransform;
	swapchain_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchain_info.presentMode = m_present_mode;
	swapchain_info.clipped = VK_TRUE;
	swapchain_info.oldSwapchain = VK_NULL_HANDLE;

	for (usize i = 0; i < m_image_views.size(); ++i) {
		vkDestroyImageView(s_vulkan_data->device, m_image_views[i], s_vulkan_data->allocator);
	}
	m_image_views.clear();
	m_images.clear();

	if (m_swapchain_handle != VK_NULL_HANDLE) {
		vkDestroySwapchainKHR(s_vulkan_data->device, m_swapchain_handle, s_vulkan_data->allocator);
		m_swapchain_handle = VK_NULL_HANDLE;
	}

	if (auto r = vkCreateSwapchainKHR(s_vulkan_data->device, &swapchain_info, s_vulkan_data->allocator, &m_swapchain_handle); r != VK_SUCCESS) {
		HC_LOG_ERROR_TAG("VULKAN", "vkCreateSwapchainKHR() failed with Vulkan code: %s", vk_result_to_string(r));
		return RendererResult::rhi_resource_creation_failed;
	}

	uint32 images_count = 0;
	HC_VULKAN_CHECK(vkGetSwapchainImagesKHR(s_vulkan_data->device, m_swapchain_handle, &images_count, nullptr));

	m_images.set_size_uninitialized(images_count);
	HC_VULKAN_CHECK(vkGetSwapchainImagesKHR(s_vulkan_data->device, m_swapchain_handle, &images_count, m_images.data()));

	m_image_views.set_size_uninitialized(m_images.size());
	for (usize i = 0; i < m_images.size(); ++i) {
		VkImageViewCreateInfo image_view_info = {};
		image_view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		image_view_info.image = m_images[i];
		image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		image_view_info.format = m_surface_format.format;
		image_view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_view_info.subresourceRange.baseMipLevel = 0;
		image_view_info.subresourceRange.levelCount = 1;
		image_view_info.subresourceRange.baseArrayLayer = 0;
		image_view_info.subresourceRange.layerCount = 1;

		if (auto r = vkCreateImageView(s_vulkan_data->device, &image_view_info, s_vulkan_data->allocator, &m_image_views[i]); r != VK_SUCCESS) {
			HC_LOG_ERROR_TAG("VULKAN", "Failed to create the swapchain's Vulkan image views!");
			return RendererResult::rhi_resource_creation_failed;
		}
	}

	return RendererResult::success;
}

RendererResult VulkanRenderer::pick_swapchain_surface_format(VkSurfaceKHR surface_handle, VkSurfaceFormatKHR* out_format)
{
	uint32 surface_formats_count = 0;
	HC_VULKAN_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(s_vulkan_data->physical_device, surface_handle, &surface_formats_count, nullptr));
	if (surface_formats_count == 0) {
		HC_LOG_ERROR_TAG("VULKAN", "No physical device surface formats were found!");
		return RendererResult::missing_format;
	}

	Array<VkSurfaceFormatKHR> surface_formats;
	surface_formats.set_size_uninitialized(surface_formats_count);
	HC_VULKAN_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(s_vulkan_data->physical_device, surface_handle, &surface_formats_count, surface_formats.data()));

	for (usize i = 0; i < surface_formats.size(); ++i) {
		VkSurfaceFormatKHR& format = surface_formats[i];
		if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) {
			*out_format = format;
			return RendererResult::success;
		}
	}

	// The best approach would be to start ranking each surface format and choose the one with the heights score,
	//   just as the physical device selection is performed.
	// But, just picking the first format in the list is usually fine.
	*out_format = surface_formats[0];
	return RendererResult::success;
}

RendererResult VulkanRenderer::pick_swapchain_present_mode(VkSurfaceKHR surface_handle, VkPresentModeKHR* out_present_mode)
{
	uint32 present_modes_count = 0;
	HC_VULKAN_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(s_vulkan_data->physical_device, surface_handle, &present_modes_count, nullptr));
	if (present_modes_count == 0) {
		HC_LOG_ERROR_TAG("VULKAN", "No physical device present modes were found!");
		return RendererResult::missing_format;
	}

	Array<VkPresentModeKHR> present_modes;
	present_modes.set_size_uninitialized(present_modes_count);
	HC_VULKAN_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(s_vulkan_data->physical_device, surface_handle, &present_modes_count, present_modes.data()));

	for (usize i = 0; i < present_modes.size(); ++i) {
		if (present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
			*out_present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
			return RendererResult::success;
		}
	}

	// VK_PRESENT_MODE_FIFO_KHR is required by the Vulkan specification to exist.
	// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPresentModeKHR.html
	*out_present_mode = VK_PRESENT_MODE_FIFO_KHR;
	return RendererResult::success;
}

} // namespace HC