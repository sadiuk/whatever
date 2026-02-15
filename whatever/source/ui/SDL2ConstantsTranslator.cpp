#include "SDL2ConstantsTranslator.h"
#include "SDL2/SDL_keyboard.h"
namespace wtv
{
	MouseButton SDL2ConstantsTranslator::GetMouseButton(int sdlMouseButton)
	{
		switch (sdlMouseButton)
		{
			case SDL_BUTTON_LEFT:
				return MouseButton::Left;
			case SDL_BUTTON_MIDDLE:
				return MouseButton::Middle;
			case SDL_BUTTON_RIGHT:
				return MouseButton::Right;
			default:
				return MouseButton::Undefined;
		}
	}

	Key SDL2ConstantsTranslator::GetKey(int sdlKeycode)
	{
		switch (sdlKeycode)
		{
			// Letters
		case SDLK_a: return Key::A;
		case SDLK_b: return Key::B;
		case SDLK_c: return Key::C;
		case SDLK_d: return Key::D;
		case SDLK_e: return Key::E;
		case SDLK_f: return Key::F;
		case SDLK_g: return Key::G;
		case SDLK_h: return Key::H;
		case SDLK_i: return Key::I;
		case SDLK_j: return Key::J;
		case SDLK_k: return Key::K;
		case SDLK_l: return Key::L;
		case SDLK_m: return Key::M;
		case SDLK_n: return Key::N;
		case SDLK_o: return Key::O;
		case SDLK_p: return Key::P;
		case SDLK_q: return Key::Q;
		case SDLK_r: return Key::R;
		case SDLK_s: return Key::S;
		case SDLK_t: return Key::T;
		case SDLK_u: return Key::U;
		case SDLK_v: return Key::V;
		case SDLK_w: return Key::W;
		case SDLK_x: return Key::X;
		case SDLK_y: return Key::Y;
		case SDLK_z: return Key::Z;

			// Numbers
		case SDLK_0: return Key::Num0;
		case SDLK_1: return Key::Num1;
		case SDLK_2: return Key::Num2;
		case SDLK_3: return Key::Num3;
		case SDLK_4: return Key::Num4;
		case SDLK_5: return Key::Num5;
		case SDLK_6: return Key::Num6;
		case SDLK_7: return Key::Num7;
		case SDLK_8: return Key::Num8;
		case SDLK_9: return Key::Num9;

			// Function Keys
		case SDLK_F1: return Key::F1;
		case SDLK_F2: return Key::F2;
		case SDLK_F3: return Key::F3;
		case SDLK_F4: return Key::F4;
		case SDLK_F5: return Key::F5;
		case SDLK_F6: return Key::F6;
		case SDLK_F7: return Key::F7;
		case SDLK_F8: return Key::F8;
		case SDLK_F9: return Key::F9;
		case SDLK_F10: return Key::F10;
		case SDLK_F11: return Key::F11;
		case SDLK_F12: return Key::F12;

			// Control Keys
		case SDLK_ESCAPE: return Key::Escape;
		case SDLK_TAB: return Key::Tab;
		case SDLK_CAPSLOCK: return Key::CapsLock;
		case SDLK_LSHIFT: return Key::LeftShift;
		case SDLK_RSHIFT: return Key::RightShift;
		case SDLK_LCTRL: return Key::LeftControl;
		case SDLK_RCTRL: return Key::RightControl;
		case SDLK_LALT: return Key::LeftAlt;
		case SDLK_RALT: return Key::RightAlt;
		case SDLK_LGUI: return Key::LeftSuper;
		case SDLK_RGUI: return Key::RightSuper;
		case SDLK_MENU: return Key::Menu;
		case SDLK_RETURN: return Key::Enter;
		case SDLK_BACKSPACE: return Key::Backspace;
		case SDLK_DELETE: return Key::Delete;
		case SDLK_INSERT: return Key::Insert;
		case SDLK_HOME: return Key::Home;
		case SDLK_END: return Key::End;
		case SDLK_PAGEUP: return Key::PageUp;
		case SDLK_PAGEDOWN: return Key::PageDown;

			// Arrow Keys
		case SDLK_UP: return Key::Up;
		case SDLK_DOWN: return Key::Down;
		case SDLK_LEFT: return Key::Left;
		case SDLK_RIGHT: return Key::Right;

			// Numpad
		case SDLK_NUMLOCKCLEAR: return Key::NumLock;
		case SDLK_KP_DIVIDE: return Key::NumpadDivide;
		case SDLK_KP_MULTIPLY: return Key::NumpadMultiply;
		case SDLK_KP_MINUS: return Key::NumpadMinus;
		case SDLK_KP_PLUS: return Key::NumpadPlus;
		case SDLK_KP_ENTER: return Key::NumpadEnter;
		case SDLK_KP_PERIOD: return Key::NumpadDecimal;
		case SDLK_KP_0: return Key::Numpad0;
		case SDLK_KP_1: return Key::Numpad1;
		case SDLK_KP_2: return Key::Numpad2;
		case SDLK_KP_3: return Key::Numpad3;
		case SDLK_KP_4: return Key::Numpad4;
		case SDLK_KP_5: return Key::Numpad5;
		case SDLK_KP_6: return Key::Numpad6;
		case SDLK_KP_7: return Key::Numpad7;
		case SDLK_KP_8: return Key::Numpad8;
		case SDLK_KP_9: return Key::Numpad9;

			// Special Keys
		case SDLK_SPACE: return Key::Space;
		case SDLK_QUOTE: return Key::Apostrophe;
		case SDLK_COMMA: return Key::Comma;
		case SDLK_MINUS: return Key::Minus;
		case SDLK_PERIOD: return Key::Period;
		case SDLK_SLASH: return Key::Slash;
		case SDLK_SEMICOLON: return Key::Semicolon;
		case SDLK_EQUALS: return Key::Equal;
		case SDLK_LEFTBRACKET: return Key::LeftBracket;
		case SDLK_BACKSLASH: return Key::Backslash;
		case SDLK_RIGHTBRACKET: return Key::RightBracket;
		case SDLK_BACKQUOTE: return Key::GraveAccent;

			// Media Keys
		case SDLK_PRINTSCREEN: return Key::PrintScreen;
		case SDLK_SCROLLLOCK: return Key::ScrollLock;
		case SDLK_PAUSE: return Key::Pause;

			// Unknown key
		default: return Key::Undefined;
		}
	}


}