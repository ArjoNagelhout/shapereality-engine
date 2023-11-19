//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef BORED_ENGINE_COMMAND_BUFFER_H
#define BORED_ENGINE_COMMAND_BUFFER_H

#include "renderer.h"

namespace renderer
{
	class CommandBuffer;

	class CommandBufferImplementation
	{
	public:
		explicit CommandBufferImplementation(CommandBuffer* commandBuffer);
		virtual ~CommandBufferImplementation();

	protected:
		CommandBuffer* pCommandBuffer;
	};

	class CommandBuffer : public RendererObject
	{
	public:
		explicit CommandBuffer();
		~CommandBuffer() override;

		void onRendererBackendChanged(RendererBackendType const& rendererBackendType) override;

		void clear();

		void blit();
		void copyTexture();
		void drawRenderer();
		void drawRendererList();
		void setRenderTarget();

	private:
		std::unique_ptr<CommandBufferImplementation> pImplementation;

	};
}

#endif //BORED_ENGINE_COMMAND_BUFFER_H
