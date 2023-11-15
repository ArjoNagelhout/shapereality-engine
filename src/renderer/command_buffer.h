//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef BORED_ENGINE_COMMAND_BUFFER_H
#define BORED_ENGINE_COMMAND_BUFFER_H

namespace renderer
{
	class CommandBuffer
	{
	public:
		virtual ~CommandBuffer();

		virtual void setVertexBuffer();
		virtual void setFragmentTexture();
		virtual void setCullMode();
	};
}

#endif //BORED_ENGINE_COMMAND_BUFFER_H
