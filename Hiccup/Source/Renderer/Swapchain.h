// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/Core.h"
#include "Renderer.h"

namespace HC
{

class RenderContext;

struct SwapchainDescription
{
	RenderContext* owning_context = nullptr;
};

class Swapchain
{
	HC_RENDERER_OBJECT(Swapchain)

public:
	static RendererResult create(const SwapchainDescription& description, UniquePtr<Swapchain>* out_swapchain);
};

} // namespace HC