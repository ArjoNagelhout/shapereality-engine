//
// Created by Arjo Nagelhout on 17/12/2023.
//

#include "input.h"

#include <cassert>

namespace input
{
	MouseInputEvent::MouseInputEvent() : mouseButton(MouseButton::Left)
	{

	}

	MouseInputEvent::~MouseInputEvent() = default;

	void IInputDelegate::onEvent(input::InputEvent const& event)
	{
		assert(false && "base virtual method should not be called");
	}

	IInputDelegate* Input::getDelegate() const
	{
		return pDelegate;
	}

	void Input::onEvent(const input::InputEvent& event)
	{
		if (pDelegate == nullptr)
		{
			return;
		}

		pDelegate->onEvent(event);
	}

	void Input::setDelegate(input::IInputDelegate* delegate)
	{
		assert(delegate != this && "cyclical reference in input delegate");
		pDelegate = delegate;
	}
}