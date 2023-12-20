//
// Created by Arjo Nagelhout on 17/12/2023.
//

#ifndef BORED_ENGINE_INPUT_TYPES_H
#define BORED_ENGINE_INPUT_TYPES_H

#include <string>
#include <sstream>

namespace input
{
	enum class MouseButton : int
	{
		None = 0,
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

	enum class MouseEventType : int
	{
		Up = 0,
		Down,
		Moved,
		Entered,
		Exited,
		Dragged, // when down and over drag threshold
	};
	constexpr static const char* mouseEventTypeStrings[]{
		"Up",
		"Down",
		"Moved",
		"Entered",
		"Exited",
		"Dragged"
	};

	constexpr static const char* toString(MouseEventType value)
	{
		return mouseEventTypeStrings[static_cast<int>(value)];
	}

	struct MouseEvent final
	{
		MouseEventType type;
		MouseButton mouseButton; // which mouse button is pressed
		float x;
		float y;
	};

	struct ScrollEvent final
	{
		float x;
		float y;
	};

	enum class KeyCode : int
	{
		None = 0,
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
	constexpr static const char* keyStrings[]{
		"None",
		"Space",
		"Enter",
		"Tab",
		"Backquote",
		"Quote",
		"Semicolon",
		"Comma",
		"Period",
		"Slash",
		"Backslash",
		"LeftBracket",
		"RightBracket",
		"Minus",
		"Equals",
		"A",
		"B",
		"C",
		"D",
		"E",
		"F",
		"G",
		"H",
		"I",
		"J",
		"K",
		"L",
		"M",
		"N",
		"O",
		"P",
		"Q",
		"R",
		"S",
		"T",
		"U",
		"V",
		"W",
		"X",
		"Y",
		"Z",
		"Digit1",
		"Digit2",
		"Digit3",
		"Digit4",
		"Digit5",
		"Digit6",
		"Digit7",
		"Digit8",
		"Digit0",
		"LeftShift",
		"RightShift",
		"LeftAlt",
		"AltGr", // same as RightAlt
		"RightAlt",
		"LeftCtrl",
		"LeftApple", // same as LeftMeta
		"LeftWindows",
		"LeftMeta",
		"LeftCommand", // same as LeftMeta
		"RightCommand", // same as RightMeta
		"RightApple", // same as RightMeta
		"RightWindows", // same as RightMeta
		"RightMeta",
		"ContextMenu",
		"Escape",
		"LeftArrow",
		"RightArrow",
		"UpArrow",
		"DownArrow",
		"Backspace",
		"PageDown",
		"PageUp",
		"Home",
		"End",
		"Insert",
		"Delete",
		"CapsLock",
		"NumLock",
		"PrintScreen",
		"ScrollLock",
		"Pause",
		"NumpadEnter",
		"NumpadDivide",
		"NumpadMultiply",
		"NumpadPlus",
		"NumpadMinus",
		"NumpadPeriod",
		"NumpadEquals",
		"Numpad0",
		"Numpad1",
		"Numpad2",
		"Numpad3",
		"Numpad4",
		"Numpad5",
		"Numpad6",
		"Numpad7",
		"Numpad8",
		"Numpad9",
		"F1",
		"F2",
		"F3",
		"F4",
		"F5",
		"F6",
		"F7",
		"F8",
		"F9",
		"F10",
		"F11",
		"F12",
		"OEM1",
		"OEM2",
		"OEM3",
		"OEM4",
		"OEM5",
		"IMESelected"
	};
	constexpr static const char* toString(KeyCode value)
	{
		return keyStrings[static_cast<int>(value)];
	}

	enum class KeyboardEventType
	{
		Up,
		Down
	};
	constexpr static char const* keyboardEventTypeStrings[]{
		"Up",
		"Down"
	};
	constexpr static char const* toString(KeyboardEventType value)
	{
		return keyboardEventTypeStrings[static_cast<int>(value)];
	}

	enum KeyboardModifier_ : int
	{
		KeyboardModifier_None = 0,
		KeyboardModifier_CapsLock = 1 << 0,
		KeyboardModifier_Shift = 1 << 1,
		KeyboardModifier_Control = 1 << 2,
		KeyboardModifier_Option = 1 << 3,
		KeyboardModifier_Meta = 1 << 4, // on macOS: Command key, windows: Windows key
		KeyboardModifier_Last = KeyboardModifier_Meta
	};
	constexpr static char const* keyboardModifierStrings[]{
		"None",
		"CapsLock",
		"Shift",
		"Control",
		"Option",
		"Meta"
	};
	static std::string toString(KeyboardModifier_ value)
	{
		std::stringstream s{};
		int j = 1;
		for (int i = 1; i <= KeyboardModifier_Last; i<<=1)
		{
			if ((value & i) != 0)
			{
				s << keyboardModifierStrings[j];
			}
			if (i < KeyboardModifier_Last)
			{
				s << ", ";
			}
			j++;

		}
		return s.str();
	}

	struct KeyboardEvent final
	{
		KeyboardEventType type;

		// non-localized keycode.
		// prefer this for game input and keyboard shortcuts
		KeyCode keyCode;

		// modifier-keys
		KeyboardModifier_ modifiers;

		// localized text input string, encoded in utf-8
		// prefer this for text input
		//std::string characters;
	};

	enum class InputEventType
	{
		None,
		Mouse,
		Scroll,
		Keyboard,
	};
	constexpr static const char* inputEventTypeStrings[]{
		"None",
		"Mouse",
		"Scroll",
		"Keyboard"
	};

	constexpr static const char* toString(InputEventType value)
	{
		return inputEventTypeStrings[static_cast<int>(value)];
	}

	struct InputEvent
	{
		InputEventType type;

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
