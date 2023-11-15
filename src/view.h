#ifndef BORED_ENGINE_VIEW_H
#define BORED_ENGINE_VIEW_H

#include <memory>

namespace engine
{
	class View
	{
	public:
		explicit View();
		virtual ~View();

	protected:
		struct Implementation;

		std::unique_ptr<Implementation> pImpl;

		friend class Window;
	};
}

#endif //BORED_ENGINE_VIEW_H
