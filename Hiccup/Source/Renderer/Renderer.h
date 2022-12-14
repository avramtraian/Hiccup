// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/Core.h"

#define HC_RENDERER_OBJECT(TYPE_NAME)   \
	public:                             \
		HC_NON_COPIABLE(TYPE_NAME)      \
		HC_NON_MOVABLE(TYPE_NAME)       \
	public:                             \
		virtual ~TYPE_NAME() = default; \
	protected:                          \
		TYPE_NAME() = default;          \
	private:

namespace HC
{

enum class RendererResult : uint32
{
	success = 0,
	unknown = 1,

	// High-level renderer results.
	unsupported_rhi,
	no_suitable_gpu,
	rhi_resource_creation_failed,
	invalid_description,

	// Low-level renderer results.
	extension_missing,
	missing_format,
	out_of_memory,
};

enum class RHI : uint8
{
	none, vulkan, d3d12, d3d11, metal, opengl
};

struct RendererCapabilities
{

};

struct RendererDescription
{
	RHI rhi = RHI::none;
};

class Renderer
{
public:
	static bool initialize(const RendererDescription& description);
	static void shutdown();

public:
	HC_API static const RendererCapabilities& get_capabilities();

private:
	static RendererCapabilities& capabilities();

	// The render contexts holds the actual OS window context, as well as the swapchain.
	// Both of these functions are called internally from the RHI specific renderer.
	// 
	// ---- VULKAN ----
	// Vulkan requires the OS window context, the surface, in order to create the logical device.
	// But the Vulkan swapchain requires the logical device. So, the context creation is divided
	//   in two stages. The first one, create_render_contexts(), obtains the surface, and the
	//   second one, create_swapchains(), creates the actual swapchain.
	static bool create_render_contexts();
	static bool create_swapchains();

	static uint32 get_next_render_context_index();
	static const class RenderContext* get_render_context(uint32 index);

private:
	friend class VulkanRenderer;
};

#define HC_RENDERER_RESULT_CASE(RENDERER_RESULT) case HC::RendererResult::RENDERER_RESULT: return #RENDERER_RESULT

ALWAYS_INLINE constexpr const char* renderer_result_to_string(RendererResult result)
{
	switch (result)
	{
		HC_RENDERER_RESULT_CASE(success);
		HC_RENDERER_RESULT_CASE(unknown);

		HC_RENDERER_RESULT_CASE(unsupported_rhi);
		HC_RENDERER_RESULT_CASE(no_suitable_gpu);
		HC_RENDERER_RESULT_CASE(rhi_resource_creation_failed);
		HC_RENDERER_RESULT_CASE(invalid_description);

		HC_RENDERER_RESULT_CASE(extension_missing);
		HC_RENDERER_RESULT_CASE(missing_format);
		HC_RENDERER_RESULT_CASE(out_of_memory);
	}

	return "undefined";
}

#undef HC_RENDERER_RESULT_CASE

} // namespace HC