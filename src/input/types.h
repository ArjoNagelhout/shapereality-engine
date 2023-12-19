//
// Created by Arjo Nagelhout on 17/12/2023.
//

#ifndef BORED_ENGINE_INPUT_TYPES_H
#define BORED_ENGINE_INPUT_TYPES_H

#include <string>

namespace input
{
	enum class MouseButton
	{
		None,
		Left,
		Right,
		Middle
	};
	constexpr static const char* mouseButtonStrings[]{
		"None",
		"Left",
		"Right",
		"Middle"
	};
	constexpr static const char* toString(MouseButton value)
	{
		return mouseButtonStrings[static_cast<int>(value)];
	}

	struct MouseEvent final
	{
		enum class Type : int
		{
			Up,
			Down,
			Moved,
			Entered,
			Exited,
			Dragged // when down and over drag threshold
		};
		constexpr static const char* typeStrings[]{
			"Up",
			"Down",
			"Moved",
			"Entered",
			"Exited",
			"Dragged"
		};
		constexpr static const char* toString(Type value)
		{
			return typeStrings[static_cast<int>(value)];
		}

		Type type;
		MouseButton mouseButton; // which mouse button is pressed
		float x;
		float y;
	};

	struct ScrollEvent final
	{
		float x;
		float y;
	};

	enum class Key
	{
		None,
		Space,
		Enter,
		Tab,
		Backquote,
		Quote,
		Semicolon,
		Comma,
		Period,
		Slash,
		Backslash,
		LeftBracket,
		RightBracket,
		Minus,
		Equals,
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		Digit1,
		Digit2,
		Digit3,
		Digit4,
		Digit5,
		Digit6,
		Digit7,
		Digit8,
		Digit0,
		LeftShift,
		RightShift,
		LeftAlt,
		AltGr, // same as RightAlt
		RightAlt,
		LeftCtrl,
		LeftApple, // same as LeftMeta
		LeftWindows,
		LeftMeta,
		LeftCommand, // same as LeftMeta
		RightCommand, // same as RightMeta
		RightApple, // same as RightMeta
		RightWindows, // same as RightMeta
		RightMeta,
		ContextMenu,
		Escape,
		LeftArrow,
		RightArrow,
		UpArrow,
		DownArrow,
		Backspace,
		PageDown,
		PageUp,
		Home,
		End,
		Insert,
		Delete,
		CapsLock,
		NumLock,
		PrintScreen,
		ScrollLock,
		Pause,
		NumpadEnter,
		NumpadDivide,
		NumpadMultiply,
		NumpadPlus,
		NumpadMinus,
		NumpadPeriod,
		NumpadEquals,
		Numpad0,
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		OEM1,
		OEM2,
		OEM3,
		OEM4,
		OEM5,
		IMESelected
	};

	struct KeyboardEvent final
	{
		enum class Type
		{
			Up,
			Down
		};
		constexpr static const char* typeStrings[]{
			"Up",
			"Down"
		};
		constexpr static const char* toString(Type value)
		{
			return typeStrings[static_cast<int>(value)];
		}

		Type type;
		Key key;
	};

	struct InputEvent
	{
		enum class Type
		{
			None,
			Mouse,
			Scroll,
			Keyboard,
		};
		constexpr static const char* typeStrings[]{
			"None",
			"Mouse",
			"Scroll",
			"Keyboard"
		};
		constexpr static const char* toString(Type value)
		{
			return typeStrings[static_cast<int>(value)];
		}

		Type type;
		union
		{
			MouseEvent mouse;
			ScrollEvent scroll;
			KeyboardEvent keyboard;
		};

		[[nodiscard]] std::string toString() const;
	};

	class IInputDelegate
	{
	public:
		virtual void onEvent(InputEvent const& event);
	};
}

#endif //BORED_ENGINE_INPUT_TYPES_H
