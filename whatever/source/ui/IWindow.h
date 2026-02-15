#pragma once
#include "util/RefPtr.h"
#include "WindowConstants.h"

#include "glm/glm.hpp"

#include <string>
#include <vector>

namespace wtv
{
	class IWindow;

	struct IWindowEventListener : public IReferenceCounted
	{
		virtual void OnWindowClose(IWindow* window) {};
		virtual void OnMouseMove(IWindow* window, const glm::ivec2& newMousePos) {}
		virtual void OnMouseButtonStateChange(IWindow* window, MouseButton button, MouseButtonState state) {};
		virtual void OnKeyStateChange(IWindow* window, Key key, KeyState state) {};
		virtual ~IWindowEventListener() = default;
	};

	class IWindow : public IReferenceCounted
	{
		std::vector<RefPtr<IWindowEventListener>> m_eventListeners;
	public:
		struct CreationParams
		{
			std::string caption;
			glm::uvec2 size;
			glm::ivec2 windowPos;
		};
		virtual bool IsOpen() = 0;
		virtual void* GetNativeHandle() = 0;
		virtual void Update() = 0;
		void AddEventListener(const RefPtr<IWindowEventListener>& listener)
		{
			m_eventListeners.push_back(listener);
		}
		virtual glm::ivec2 GetMousePosition() = 0;
	protected:
		void NotifyWindowClose()
		{
			for (auto& listener : m_eventListeners)
				listener->OnWindowClose(this);
		}
		void NotifyMouseMove(const glm::ivec2& newMousePos)
		{
			for (auto& listener : m_eventListeners)
				listener->OnMouseMove(this, newMousePos);
		}
		void NotifyMouseButtonStateChange(MouseButton button, MouseButtonState state)
		{
			for (auto& listener : m_eventListeners)
				listener->OnMouseButtonStateChange(this, button, state);
		}
		void NotifyKeyStateChange(Key key, KeyState state)
		{
			for (auto& listener : m_eventListeners)
				listener->OnKeyStateChange(this, key, state);
		}
	};
}