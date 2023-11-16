#ifndef BORED_ENGINE_COMPONENT_H
#define BORED_ENGINE_COMPONENT_H

#include "object.h"

namespace scene
{
	class Component : public Object
	{
	public:
		explicit Component();
		virtual ~Component();

	protected:

	private:

	};
}

#endif //BORED_ENGINE_COMPONENT_H
