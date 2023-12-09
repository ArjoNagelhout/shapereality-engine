
#ifndef BORED_ENGINE_RENDER_PASS_H
#define BORED_ENGINE_RENDER_PASS_H

#include "types.h"

#include <vector>

namespace graphics
{
	enum class LoadAction
	{
		DontCare,
		Load,
		Clear
	};

	enum class StoreAction
	{
		DontCare,
		Store,
		MultisampleResolve
	};

	enum class MultisampleDepthResolveFilter
	{
		Sample0, // No filter is applied.
		Min, // compare all depth samples in the pixel and selects the sample with the smallest value.
		Max // compare all depth samples in the pixel and selects the sample with the largest value.
	};

	struct RenderPassDescription
	{
		struct Attachment
		{
			LoadAction loadAction = LoadAction::Clear;
			StoreAction storeAction = StoreAction::DontCare;
		};

		struct ColorAttachment : public Attachment
		{
			Color clearColor{1.f, 1.f, 1.f, 1.f};
		};

		struct DepthAttachment : public Attachment
		{
			float clearDepth = 1.f;
			MultisampleDepthResolveFilter depthResolveFilter = MultisampleDepthResolveFilter::Sample0;
		};

		struct StencilAttachment : public Attachment
		{

		};

		std::vector<ColorAttachment> colorAttachments;

		DepthAttachment depthAttachment;

		StencilAttachment stencilAttachment;
	};

	class IRenderPass
	{
	public:
		virtual ~IRenderPass() = default;
	};
}

#endif //BORED_ENGINE_RENDER_PASS_H
