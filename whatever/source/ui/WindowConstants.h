#pragma once
#include <cstdint>

namespace wtv
{
	enum class MouseButton : uint8_t
	{
		Left = 0,
		Middle ,
		Right,
		Count,
		Undefined = 255
	};

	enum class MouseButtonState : uint8_t
	{
		Released = 0,
		Pressed,
		Count,
		Undefined = 255
	};


	enum class Key : uint16_t
	{
		A = 0,
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

		Num0,
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,

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

		Escape,
		Tab,
		CapsLock,
		LeftShift,
		RightShift,
		LeftControl,
		RightControl,
		LeftAlt,
		RightAlt,
		LeftSuper,
		RightSuper,
		Menu,
		Enter,
		Backspace,
		Delete,
		Insert,
		Home,
		End,
		PageUp,
		PageDown,

		Up,
		Down,
		Left,
		Right,

		NumLock,
		NumpadDivide,
		NumpadMultiply,
		NumpadMinus,
		NumpadPlus,
		NumpadEnter,
		NumpadDecimal,
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

		Space,
		Apostrophe,
		Comma,
		Minus,
		Period,
		Slash,
		Semicolon,
		Equal,
		LeftBracket,
		Backslash,
		RightBracket,
		GraveAccent,

		PrintScreen,
		ScrollLock,
		Pause,

		Count,
		Undefined = 0xfffffff,
	};

	enum class KeyState : uint8_t
	{
		Released = 0,
		Pressed,
		Count,
		Undefined = 255
	};
}