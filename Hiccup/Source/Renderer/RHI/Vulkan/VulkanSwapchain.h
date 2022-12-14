// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/Core.h"
#include "VulkanDefines.h"

#include "VulkanRenderer.h"
#include "Renderer/Swapchain.h"

#include <vulkan/vulkan.h>

namespace HC
{

class VulkanRenderContext;

class VulkanSwapchain : public Swapchain
{
public:
	VulkanSwapchain(const SwapchainDescription& description, RendererResult* out_result);
	virtual ~VulkanSwapchain() override;

private:
	RendererResult query_parameters();
	RendererResult invalidate();

private:
	VkSwapchainKHR m_swapchain_handle;

	VulkanRenderContext* m_owning_context;

	uint32 m_min_image_count;
	VkSurfaceFormatKHR m_surface_format;
	VkSharingMode m_image_sharing_mode;
	uint32 m_queue_family_indices_count;
	uint32 m_queue_family_indices[2];
	VkPresentModeKHR m_present_mode;
	VkSurfaceTransformFlagBitsKHR m_pre_transform;

	VkSurfaceCapabilitiesKHR m_surface_caps;

private:
	friend class VulkanRenderContext;
};

} // namespace HC