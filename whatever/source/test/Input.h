#pragma once
#include "ui/IWindow.h"

namespace wtv
{
	class Input : public wtv::IWindowEventListener
	{
		KeyState m_keyStates[static_cast<size_t>(Key::Count)]{};
		MouseButtonState m_mouseButtonStates[static_cast<size_t>(MouseButton::Count)]{};
		glm::ivec2 m_mousePos{};
		glm::ivec2 m_relativeMousePos{ 0, 0 };
	private:
		void OnMouseMove(IWindow* window, const glm::ivec2& newMousePos) override
		{
			m_relativeMousePos = newMousePos - m_mousePos;
			m_mousePos = newMousePos;
		}
		void OnMouseButtonStateChange(IWindow* window, MouseButton button, MouseButtonState state) override
		{
			if (m_mouseButtonStates[(int)button] != state)
				m_relativeMousePos = { 0, 0 };
			m_mouseButtonStates[(int)button] = state;
		}
		void OnKeyStateChange(IWindow* window, Key key, KeyState state) override
		{
			m_keyStates[(int)key] = state;
		}
	public:
		Input(IWindow* window)
		{
			window->AddEventListener(RefPtr<Input>(this));
			m_mousePos = window->GetMousePosition();
		}

		bool IsMouseButtonDown(MouseButton button) const
		{
			return m_mouseButtonStates[(int)button] == MouseButtonState::Pressed;
		}
		bool IsKeyDown(Key key) const
		{
			return m_keyStates[(int)key] == KeyState::Pressed;
		}
		glm::ivec2 GetMousePosition() const
		{
			return m_mousePos;
		}
		glm::ivec2 GetRelativeMousePosition() const
		{
			return m_relativeMousePos;
		}
	};

}