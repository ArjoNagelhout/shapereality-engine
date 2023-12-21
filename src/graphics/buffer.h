//
// Created by Arjo Nagelhout on 19/11/2023.
//

#ifndef BORED_ENGINE_BUFFER_H
#define BORED_ENGINE_BUFFER_H

#include <cassert>
#include <cstddef>

#include "graphics.h"

namespace graphics
{
	struct BufferDescriptor
	{
		enum class Type
		{
			Index,
			Vertex,
			Uniform,
			// etc.
		};

		enum class StorageMode
		{
			Private, // only on GPU
			Shared, // on GPU and CPU
			Managed // on GPU and CPU, but memory is backed by private storage, so can still be optimised
		};

		Type type;

		StorageMode storageMode;

		// can be null
		const void* data{nullptr};

		// size of the buffer in bytes
		unsigned int length;

		// the size of each individual element in the buffer
		// useful for an index buffer for example
		size_t stride;
	};

	class IBuffer
	{
	public:
		virtual ~IBuffer() = default;

		// get the system address of the buffer's storage allocation.
		// note: if the buffer has StorageMode::Private, this returns nullptr!
		virtual void* getContents()
		{
			assert(false && "interface class method should not be called");
		}

		// inform the GPU that the CPU has modified a section of the buffer
		virtual void didModifyRange(Range range)
		{
			assert(false && "interface class method should not be called");
		}
	};
}

#endif //BORED_ENGINE_BUFFER_H
