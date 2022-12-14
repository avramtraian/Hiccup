// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "Renderer.h"
#include "Core/Application.h"

#include "RenderContext.h"
#include "Swapchain.h"

#include "RHI/Vulkan/VulkanRenderer.h"

#include "RHI/Vulkan/VulkanRenderContext.h"
#include "RHI/Vulkan/VulkanSwapchain.h"

namespace HC
{

struct RendererData
{
	RHI rhi;

	RendererCapabilities capabilities;

	Array<UniquePtr<RenderContext>> contexts;
};
static_internal RendererData* s_renderer_data = nullptr;

bool Renderer::initialize(const RendererDescription& description)
{
	s_renderer_data = hc_new RendererData();
	s_renderer_data->rhi = description.rhi;

	switch (s_renderer_data->rhi)
	{
		case RHI::vulkan:
		{
			VulkanRenderer::initialize(description);
			break;
		}
	}

	return true;
}

void Renderer::shutdown()
{
	// Releases all the rendering contexts.
	s_renderer_data->contexts.clear();

	switch (s_renderer_data->rhi)
	{
		case RHI::vulkan:
		{
			VulkanRenderer::shutdown();
			break;
		}
	}

	hc_delete s_renderer_data;
	s_renderer_data = nullptr;
}

const RendererCapabilities& Renderer::get_capabilities()
{
	return s_renderer_data->capabilities;
}

RendererCapabilities& Renderer::capabilities()
{
	return s_renderer_data->capabilities;
}

bool Renderer::create_render_contexts()
{
	HC_LOG_INFO_TAG("RENDERER", "Creating the primary window's render context.");
	Window* primary_window = Application::get()->get_primary_window();

	RenderContextDescription context_desc = {};
	context_desc.owner = primary_window;
	context_desc.is_primary_context = true;

	UniquePtr<RenderContext> context;
	RendererResult result = RenderContext::create(context_desc, &context);

	if (result == RendererResult::success) {
		s_renderer_data->contexts.add(Types::move(context));

		HC_LOG_INFO_TAG("RENDERER", "All render contexts were created successfully (%u contexts).", s_renderer_data->contexts.size());
		return true;
	}
	else {

		HC_LOG_ERROR_TAG("RENDERER", "Failed to create the rendering context! Code: %s", renderer_result_to_string(result));
		return false;
	}
}

bool Renderer::create_swapchains()
{
	bool swapchains_created_successfully = true;
	uint32 swapchains_created_count = 0;

	for (usize i = 0; i < s_renderer_data->contexts.size(); ++i) {
		RendererResult result = s_renderer_data->contexts[i]->create_swapchain();

		if (result == RendererResult::success) {
			swapchains_created_count++;
		}
		else {
			swapchains_created_successfully = false;
			HC_LOG_ERROR_TAG("RENDERER", "Failed to create the render context's swapchain! Code: %s", renderer_result_to_string(result));
		}
	}

	if (swapchains_created_successfully) {
		HC_LOG_INFO_TAG("RENDERER", "All render contexts' swapchains were created successfully (%u swapchains).", swapchains_created_count);
	}
	return swapchains_created_successfully;
}

uint32 Renderer::get_next_render_context_index()
{
	return (uint32)s_renderer_data->contexts.size();
}

const RenderContext* Renderer::get_render_context(uint32 index)
{
	return s_renderer_data->contexts[index].get();
}

RendererResult RenderContext::create(const RenderContextDescription& description, UniquePtr<RenderContext>* out_render_context)
{
	RendererResult result = RendererResult::unsupported_rhi;

	switch (s_renderer_data->rhi)
	{
		case RHI::vulkan:
		{
			*out_render_context = UniquePtr<VulkanRenderContext>::create(description, &result).as<RenderContext>();
			break;
		}
	}

	if (result != RendererResult::success)
	{
		HC_LOG_ERROR_TAG("RENDERER", "Failed to create a RenderingContext! Code: %s", renderer_result_to_string(result));
		out_render_context->release();
	}
	return result;
}

RendererResult Swapchain::create(const SwapchainDescription& description, UniquePtr<Swapchain>* out_swapchain)
{
	RendererResult result = RendererResult::unsupported_rhi;

	switch (s_renderer_data->rhi) {
		case RHI::vulkan: {
			*out_swapchain = UniquePtr<VulkanSwapchain>::create(description, &result).as<Swapchain>();
			break;
		}
	}

	if (result != RendererResult::success) {
		HC_LOG_ERROR_TAG("RENDERER", "Failed to create a Swapchain! Code: %s", renderer_result_to_string(result));
		out_swapchain->release();
	}
	return result;
}

} // namespace HC