// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/Core.h"
#include "Renderer.h"

namespace HC
{

class Window;
class Swapchain;
struct SwapchainDescription;

struct RenderContextDescription
{
	Window* owner = nullptr;
	bool is_primary_context = false;
};

class RenderContext
{
	HC_RENDERER_OBJECT(RenderContext)

public:
	static RendererResult create(const RenderContextDescription& description, UniquePtr<RenderContext>* out_render_context);;

public:
	virtual Swapchain* get_swapchain() = 0;
	virtual const Swapchain* get_swapchain() const = 0;

	virtual RendererResult create_swapchain() = 0;
};

} // namespace HC