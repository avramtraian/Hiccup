// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/Core.h"
#include "VulkanDefines.h"

#include "Renderer/Renderer.h"
#include "Renderer/RenderContext.h"

#include "VulkanSwapchain.h"

#include <vulkan/vulkan.h>

namespace HC
{

class VulkanRenderContext : public RenderContext
{
public:
	VulkanRenderContext(const RenderContextDescription& description, RendererResult* out_result);
	virtual ~VulkanRenderContext() override;

public:
	virtual Swapchain* get_swapchain() override { return m_swapchain.get(); }
	virtual const Swapchain* get_swapchain() const override { return m_swapchain.get(); }

	virtual RendererResult create_swapchain() override;

private:
	VkSurfaceKHR m_surface_handle;

	Window* m_owner;

	UniquePtr<VulkanSwapchain> m_swapchain;

private:
	friend class VulkanRenderer;
	friend class VulkanSwapchain;
};

} // namespace HC