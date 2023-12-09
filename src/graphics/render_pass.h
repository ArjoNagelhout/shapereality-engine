
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

	enum class MultisampleDepthResolveFilter // (MSAA)
	{
		Sample0, // No filter is applied.
		Min, // compare all depth samples in the pixel and selects the sample with the smallest value.
		Max // compare all depth samples in the pixel and selects the sample with the largest value.
	};

	// a group of render targets that hold the results of the render pass
	struct RenderPassDescriptor
	{
		struct AttachmentDescriptor
		{
			AttachmentDescriptor() = default;
			AttachmentDescriptor(LoadAction loadAction, StoreAction storeAction) : loadAction(loadAction), storeAction(storeAction) // to enable overwriting defaults
			{}

			LoadAction loadAction = LoadAction::Clear;
			StoreAction storeAction = StoreAction::DontCare;
			uint8_t face = 0; // face of cube texture
			uint8_t mipLevel = 0; // mip level of texture
			uint8_t layer = 0; // layer of texture array
		};

		struct ColorAttachmentDescriptor : public AttachmentDescriptor
		{
			ColorAttachmentDescriptor() : AttachmentDescriptor(LoadAction::DontCare, StoreAction::Store) {} // different defaults
			Color clearColor{1.f, 1.f, 1.f, 1.f};
		};

		struct DepthAttachmentDescriptor : public AttachmentDescriptor
		{
			float clearDepth = 1.f;
			MultisampleDepthResolveFilter depthResolveFilter = MultisampleDepthResolveFilter::Sample0;
		};

		struct StencilAttachmentDescriptor : public AttachmentDescriptor
		{
			uint8_t clearStencil = 0;
		};

		std::vector<ColorAttachmentDescriptor> colorAttachments;

		DepthAttachmentDescriptor depthAttachment;

		StencilAttachmentDescriptor stencilAttachment;
	};

	class IRenderPass
	{
	public:
		virtual ~IRenderPass() = default;
	};
}

#endif //BORED_ENGINE_RENDER_PASS_H
