//
// Created by Arjo Nagelhout on 17/12/2023.
//

#ifndef BORED_ENGINE_INPUT_H
#define BORED_ENGINE_INPUT_H

#include <memory>

#include "types.h"

namespace input
{
	class Input final : public IInputDelegate
	{
	public:
		explicit Input();
		~Input();

		//
		[[nodiscard]] IInputDelegate* getDelegate() const;

		//
		void setDelegate(IInputDelegate* delegate);

		// override the onEvent (so that it can be called by a Window)
		// this will then be propagated to the delegate of the input
		void onEvent(const input::InputEvent &event) override;

	private:
		struct Implementation;

		std::unique_ptr<Implementation> pImplementation;

		IInputDelegate* pDelegate{nullptr};
	};
}

#endif //BORED_ENGINE_INPUT_H
