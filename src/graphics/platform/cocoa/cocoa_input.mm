//
// Created by Arjo Nagelhout on 17/12/2023.
//

#include "cocoa_input.h"

#include <iostream>
#include "cocoa.h"

namespace graphics
{
	// retrieved from /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/Carbon.framework/Versions/A/Frameworks/HIToolbox.framework/Versions/A/Headers/Events.h
	enum class CocoaKeyCode : unsigned short
	{
		/*
		 *  Summary:
		 *    Virtual keycodes
		 *
		 *  Discussion:
		 *    These constants are the virtual keycodes defined originally in
		 *    Inside Mac Volume V, pg. V-191. They identify physical keys on a
		 *    keyboard. Those constants with "ANSI" in the name are labeled
		 *    according to the key position on an ANSI-standard US keyboard.
		 *    For example, kVK_ANSI_A indicates the virtual keycode for the key
		 *    with the letter 'A' in the US keyboard layout. Other keyboard
		 *    layouts may have the 'A' key label on a different physical key;
		 *    in this case, pressing 'A' will generate a different virtual
		 *    keycode.
		 */
		kVK_ANSI_A                    = 0x00,
		kVK_ANSI_S                    = 0x01,
		kVK_ANSI_D                    = 0x02,
		kVK_ANSI_F                    = 0x03,
		kVK_ANSI_H                    = 0x04,
		kVK_ANSI_G                    = 0x05,
		kVK_ANSI_Z                    = 0x06,
		kVK_ANSI_X                    = 0x07,
		kVK_ANSI_C                    = 0x08,
		kVK_ANSI_V                    = 0x09,
		kVK_ANSI_B                    = 0x0B,
		kVK_ANSI_Q                    = 0x0C,
		kVK_ANSI_W                    = 0x0D,
		kVK_ANSI_E                    = 0x0E,
		kVK_ANSI_R                    = 0x0F,
		kVK_ANSI_Y                    = 0x10,
		kVK_ANSI_T                    = 0x11,
		kVK_ANSI_1                    = 0x12,
		kVK_ANSI_2                    = 0x13,
		kVK_ANSI_3                    = 0x14,
		kVK_ANSI_4                    = 0x15,
		kVK_ANSI_6                    = 0x16,
		kVK_ANSI_5                    = 0x17,
		kVK_ANSI_Equal                = 0x18,
		kVK_ANSI_9                    = 0x19,
		kVK_ANSI_7                    = 0x1A,
		kVK_ANSI_Minus                = 0x1B,
		kVK_ANSI_8                    = 0x1C,
		kVK_ANSI_0                    = 0x1D,
		kVK_ANSI_RightBracket         = 0x1E,
		kVK_ANSI_O                    = 0x1F,
		kVK_ANSI_U                    = 0x20,
		kVK_ANSI_LeftBracket          = 0x21,
		kVK_ANSI_I                    = 0x22,
		kVK_ANSI_P                    = 0x23,
		kVK_ANSI_L                    = 0x25,
		kVK_ANSI_J                    = 0x26,
		kVK_ANSI_Quote                = 0x27,
		kVK_ANSI_K                    = 0x28,
		kVK_ANSI_Semicolon            = 0x29,
		kVK_ANSI_Backslash            = 0x2A,
		kVK_ANSI_Comma                = 0x2B,
		kVK_ANSI_Slash                = 0x2C,
		kVK_ANSI_N                    = 0x2D,
		kVK_ANSI_M                    = 0x2E,
		kVK_ANSI_Period               = 0x2F,
		kVK_ANSI_Grave                = 0x32,
		kVK_ANSI_KeypadDecimal        = 0x41,
		kVK_ANSI_KeypadMultiply       = 0x43,
		kVK_ANSI_KeypadPlus           = 0x45,
		kVK_ANSI_KeypadClear          = 0x47,
		kVK_ANSI_KeypadDivide         = 0x4B,
		kVK_ANSI_KeypadEnter          = 0x4C,
		kVK_ANSI_KeypadMinus          = 0x4E,
		kVK_ANSI_KeypadEquals         = 0x51,
		kVK_ANSI_Keypad0              = 0x52,
		kVK_ANSI_Keypad1              = 0x53,
		kVK_ANSI_Keypad2              = 0x54,
		kVK_ANSI_Keypad3              = 0x55,
		kVK_ANSI_Keypad4              = 0x56,
		kVK_ANSI_Keypad5              = 0x57,
		kVK_ANSI_Keypad6              = 0x58,
		kVK_ANSI_Keypad7              = 0x59,
		kVK_ANSI_Keypad8              = 0x5B,
		kVK_ANSI_Keypad9              = 0x5C,
		/* keycodes for keys that are independent of keyboard layout*/
		kVK_Return                    = 0x24,
		kVK_Tab                       = 0x30,
		kVK_Space                     = 0x31,
		kVK_Delete                    = 0x33,
		kVK_Escape                    = 0x35,
		kVK_Command                   = 0x37,
		kVK_Shift                     = 0x38,
		kVK_CapsLock                  = 0x39,
		kVK_Option                    = 0x3A,
		kVK_Control                   = 0x3B,
		kVK_RightCommand              = 0x36,
		kVK_RightShift                = 0x3C,
		kVK_RightOption               = 0x3D,
		kVK_RightControl              = 0x3E,
		kVK_Function                  = 0x3F,
		kVK_F17                       = 0x40,
		kVK_VolumeUp                  = 0x48,
		kVK_VolumeDown                = 0x49,
		kVK_Mute                      = 0x4A,
		kVK_F18                       = 0x4F,
		kVK_F19                       = 0x50,
		kVK_F20                       = 0x5A,
		kVK_F5                        = 0x60,
		kVK_F6                        = 0x61,
		kVK_F7                        = 0x62,
		kVK_F3                        = 0x63,
		kVK_F8                        = 0x64,
		kVK_F9                        = 0x65,
		kVK_F11                       = 0x67,
		kVK_F13                       = 0x69,
		kVK_F16                       = 0x6A,
		kVK_F14                       = 0x6B,
		kVK_F10                       = 0x6D,
		kVK_F12                       = 0x6F,
		kVK_F15                       = 0x71,
		kVK_Help                      = 0x72,
		kVK_Home                      = 0x73,
		kVK_PageUp                    = 0x74,
		kVK_ForwardDelete             = 0x75,
		kVK_F4                        = 0x76,
		kVK_End                       = 0x77,
		kVK_F2                        = 0x78,
		kVK_PageDown                  = 0x79,
		kVK_F1                        = 0x7A,
		kVK_LeftArrow                 = 0x7B,
		kVK_RightArrow                = 0x7C,
		kVK_DownArrow                 = 0x7D,
		kVK_UpArrow                   = 0x7E
	};

	KeyCode convert(unsigned short keyCode)
	{
		switch (static_cast<CocoaKeyCode>(keyCode))
		{
			case CocoaKeyCode::kVK_ANSI_A:                     return KeyCode::A;
			case CocoaKeyCode::kVK_ANSI_S:                     return KeyCode::S;
			case CocoaKeyCode::kVK_ANSI_D:                     return KeyCode::D;
			case CocoaKeyCode::kVK_ANSI_F:                     return KeyCode::F;
			case CocoaKeyCode::kVK_ANSI_H:                     return KeyCode::H;
			case CocoaKeyCode::kVK_ANSI_G:                     return KeyCode::G;
			case CocoaKeyCode::kVK_ANSI_Z:                     return KeyCode::Z;
			case CocoaKeyCode::kVK_ANSI_X:                     return KeyCode::X;
			case CocoaKeyCode::kVK_ANSI_C:                     return KeyCode::C;
			case CocoaKeyCode::kVK_ANSI_V:                     return KeyCode::V;
			case CocoaKeyCode::kVK_ANSI_B:                     return KeyCode::B;
			case CocoaKeyCode::kVK_ANSI_Q:                     return KeyCode::Q;
			case CocoaKeyCode::kVK_ANSI_W:                     return KeyCode::W;
			case CocoaKeyCode::kVK_ANSI_E:                     return KeyCode::E;
			case CocoaKeyCode::kVK_ANSI_R:                     return KeyCode::R;
			case CocoaKeyCode::kVK_ANSI_Y:                     return KeyCode::Y;
			case CocoaKeyCode::kVK_ANSI_T:                     return KeyCode::T;
			case CocoaKeyCode::kVK_ANSI_1:                     return KeyCode::Digit1;
			case CocoaKeyCode::kVK_ANSI_2:                     return KeyCode::Digit2;
			case CocoaKeyCode::kVK_ANSI_3:                     return KeyCode::Digit3;
			case CocoaKeyCode::kVK_ANSI_4:                     return KeyCode::Digit4;
			case CocoaKeyCode::kVK_ANSI_6:                     return KeyCode::Digit6;
			case CocoaKeyCode::kVK_ANSI_5:                     return KeyCode::Digit5;
			case CocoaKeyCode::kVK_ANSI_Equal:                 return KeyCode::Equals;
			case CocoaKeyCode::kVK_ANSI_9:                     return KeyCode::Digit9;
			case CocoaKeyCode::kVK_ANSI_7:                     return KeyCode::Digit7;
			case CocoaKeyCode::kVK_ANSI_Minus:                 return KeyCode::Minus;
			case CocoaKeyCode::kVK_ANSI_8:                     return KeyCode::Digit8;
			case CocoaKeyCode::kVK_ANSI_0:                     return KeyCode::Digit0;
			case CocoaKeyCode::kVK_ANSI_RightBracket:          return KeyCode::RightBracket;
			case CocoaKeyCode::kVK_ANSI_O:                     return KeyCode::O;
			case CocoaKeyCode::kVK_ANSI_U:                     return KeyCode::U;
			case CocoaKeyCode::kVK_ANSI_LeftBracket:           return KeyCode::LeftBracket;
			case CocoaKeyCode::kVK_ANSI_I:                     return KeyCode::I;
			case CocoaKeyCode::kVK_ANSI_P:                     return KeyCode::P;
			case CocoaKeyCode::kVK_ANSI_L:                     return KeyCode::L;
			case CocoaKeyCode::kVK_ANSI_J:                     return KeyCode::J;
			case CocoaKeyCode::kVK_ANSI_Quote:                 return KeyCode::Quote;
			case CocoaKeyCode::kVK_ANSI_K:                     return KeyCode::K;
			case CocoaKeyCode::kVK_ANSI_Semicolon:             return KeyCode::Semicolon;
			case CocoaKeyCode::kVK_ANSI_Backslash:             return KeyCode::Backslash;
			case CocoaKeyCode::kVK_ANSI_Comma:                 return KeyCode::Comma;
			case CocoaKeyCode::kVK_ANSI_Slash:                 return KeyCode::Slash;
			case CocoaKeyCode::kVK_ANSI_N:                     return KeyCode::N;
			case CocoaKeyCode::kVK_ANSI_M:                     return KeyCode::M;
			case CocoaKeyCode::kVK_ANSI_Period:                return KeyCode::Period;
			case CocoaKeyCode::kVK_ANSI_Grave:                 return KeyCode::Backtick;
			case CocoaKeyCode::kVK_ANSI_KeypadDecimal:         return KeyCode::NumpadPeriod;
			case CocoaKeyCode::kVK_ANSI_KeypadMultiply:        return KeyCode::NumpadMultiply;
			case CocoaKeyCode::kVK_ANSI_KeypadPlus:            return KeyCode::NumpadPlus;
			case CocoaKeyCode::kVK_ANSI_KeypadClear:           return KeyCode::None;
			case CocoaKeyCode::kVK_ANSI_KeypadDivide:          return KeyCode::NumpadDivide;
			case CocoaKeyCode::kVK_ANSI_KeypadEnter:           return KeyCode::NumpadEnter;
			case CocoaKeyCode::kVK_ANSI_KeypadMinus:           return KeyCode::NumpadMinus;
			case CocoaKeyCode::kVK_ANSI_KeypadEquals:          return KeyCode::NumpadEquals;
			case CocoaKeyCode::kVK_ANSI_Keypad0:               return KeyCode::Numpad0;
			case CocoaKeyCode::kVK_ANSI_Keypad1:               return KeyCode::Numpad1;
			case CocoaKeyCode::kVK_ANSI_Keypad2:               return KeyCode::Numpad2;
			case CocoaKeyCode::kVK_ANSI_Keypad3:               return KeyCode::Numpad3;
			case CocoaKeyCode::kVK_ANSI_Keypad4:               return KeyCode::Numpad4;
			case CocoaKeyCode::kVK_ANSI_Keypad5:               return KeyCode::Numpad5;
			case CocoaKeyCode::kVK_ANSI_Keypad6:               return KeyCode::Numpad6;
			case CocoaKeyCode::kVK_ANSI_Keypad7:               return KeyCode::Numpad7;
			case CocoaKeyCode::kVK_ANSI_Keypad8:               return KeyCode::Numpad8;
			case CocoaKeyCode::kVK_ANSI_Keypad9:               return KeyCode::Numpad9;
			case CocoaKeyCode::kVK_Return:                     return KeyCode::Enter;
			case CocoaKeyCode::kVK_Tab:                        return KeyCode::Tab;
			case CocoaKeyCode::kVK_Space:                      return KeyCode::Space;
			case CocoaKeyCode::kVK_Delete:                     return KeyCode::Delete;
			case CocoaKeyCode::kVK_Escape:                     return KeyCode::Escape;
			case CocoaKeyCode::kVK_Command:                    return KeyCode::LeftMeta;
			case CocoaKeyCode::kVK_Shift:                      return KeyCode::LeftShift;
			case CocoaKeyCode::kVK_CapsLock:                   return KeyCode::CapsLock;
			case CocoaKeyCode::kVK_Option:                     return KeyCode::LeftOption;
			case CocoaKeyCode::kVK_Control:                    return KeyCode::LeftControl;
			case CocoaKeyCode::kVK_RightCommand:               return KeyCode::RightMeta;
			case CocoaKeyCode::kVK_RightShift:                 return KeyCode::RightShift;
			case CocoaKeyCode::kVK_RightOption:                return KeyCode::RightOption;
			case CocoaKeyCode::kVK_RightControl:               return KeyCode::RightControl;
			case CocoaKeyCode::kVK_Function:                   return KeyCode::None;
			case CocoaKeyCode::kVK_F17:                        return KeyCode::F17;
			case CocoaKeyCode::kVK_VolumeUp:                   return KeyCode::None;
			case CocoaKeyCode::kVK_VolumeDown:                 return KeyCode::None;
			case CocoaKeyCode::kVK_Mute:                       return KeyCode::None;
			case CocoaKeyCode::kVK_F18:                        return KeyCode::F18;
			case CocoaKeyCode::kVK_F19:                        return KeyCode::F19;
			case CocoaKeyCode::kVK_F20:                        return KeyCode::F20;
			case CocoaKeyCode::kVK_F5:                         return KeyCode::F5;
			case CocoaKeyCode::kVK_F6:                         return KeyCode::F6;
			case CocoaKeyCode::kVK_F7:                         return KeyCode::F7;
			case CocoaKeyCode::kVK_F3:                         return KeyCode::F3;
			case CocoaKeyCode::kVK_F8:                         return KeyCode::F8;
			case CocoaKeyCode::kVK_F9:                         return KeyCode::F9;
			case CocoaKeyCode::kVK_F11:                        return KeyCode::F11;
			case CocoaKeyCode::kVK_F13:                        return KeyCode::F13;
			case CocoaKeyCode::kVK_F16:                        return KeyCode::F16;
			case CocoaKeyCode::kVK_F14:                        return KeyCode::F14;
			case CocoaKeyCode::kVK_F10:                        return KeyCode::F10;
			case CocoaKeyCode::kVK_F12:                        return KeyCode::F12;
			case CocoaKeyCode::kVK_F15:                        return KeyCode::F15;
			case CocoaKeyCode::kVK_Help:                       return KeyCode::None;
			case CocoaKeyCode::kVK_Home:                       return KeyCode::Home;
			case CocoaKeyCode::kVK_PageUp:                     return KeyCode::PageUp;
			case CocoaKeyCode::kVK_ForwardDelete:              return KeyCode::Delete;
			case CocoaKeyCode::kVK_F4:                         return KeyCode::F4;
			case CocoaKeyCode::kVK_End:                        return KeyCode::End;
			case CocoaKeyCode::kVK_F2:                         return KeyCode::F2;
			case CocoaKeyCode::kVK_PageDown:                   return KeyCode::PageDown;
			case CocoaKeyCode::kVK_F1:                         return KeyCode::F1;
			case CocoaKeyCode::kVK_LeftArrow:                  return KeyCode::LeftArrow;
			case CocoaKeyCode::kVK_RightArrow:                 return KeyCode::RightArrow;
			case CocoaKeyCode::kVK_DownArrow:                  return KeyCode::DownArrow;
			case CocoaKeyCode::kVK_UpArrow:                    return KeyCode::UpArrow;
		}
		return KeyCode::None;
	}

	[[nodiscard]] KeyboardModifier_ convert(NSEventModifierFlags flags)
	{
		int mask = KeyboardModifier_None;
		mask |= (flags & NSEventModifierFlagCapsLock) ? KeyboardModifier_CapsLock : 0;
		mask |= (flags & NSEventModifierFlagShift) ? KeyboardModifier_Shift : 0;
		mask |= (flags & NSEventModifierFlagControl) ? KeyboardModifier_Control : 0;
		mask |= (flags & NSEventModifierFlagOption) ? KeyboardModifier_Option : 0;
		mask |= (flags & NSEventModifierFlagCommand) ? KeyboardModifier_Meta : 0;
		return static_cast<KeyboardModifier_>(mask);
	}

	InputEvent createMouseEvent(NSEvent* event, MouseEventType type, MouseButton mouseButton)
	{
		float x = 0.f;
		float y = 0.f;

		if (type != MouseEventType::Entered && type != MouseEventType::Exited)
		{
			x = static_cast<float>(event.locationInWindow.x);
			y = static_cast<float>(event.locationInWindow.y);
		}

		return InputEvent{
			.type = InputEventType::Mouse,
			.mouse = MouseEvent{
				.type = type,
				.mouseButton = mouseButton,
				.x = x,
				.y = y
			}
		};
	}

	InputEvent createScrollEvent(NSEvent* event)
	{
		return InputEvent{
			.type = InputEventType::Scroll,
			.scroll = ScrollEvent{
				.x = static_cast<float>(event.scrollingDeltaX),
				.y = static_cast<float>(event.scrollingDeltaY)
			}
		};
	}

	[[nodiscard]] InputEvent createKeyboardEvent(NSEvent* event, KeyboardEventType type)
	{
		KeyCode keyCode = KeyCode::None;

		if (type != KeyboardEventType::ModifiersChanged)
		{
			if (event.isARepeat)
			{
				type = KeyboardEventType::Repeat;
			}
		}

		return InputEvent{
			.type = InputEventType::Keyboard,
			.keyboard = KeyboardEvent{
				.type = type,
				.keyCode = convert(event.keyCode),
				.modifiers = convert(event.modifierFlags)
			}
		};
	}
}