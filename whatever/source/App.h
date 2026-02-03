#pragma once
#include "graphics/IDevice.h"
#include "ui/IWindow.h"
#include "IServiceProvider.h"
#include "util/RefPtr.h"
#include "graphics/IFramebuffer.h"

#include <memory>
#include <string>
class App
{
public:
	App();
	void Run();
private:
	void Init();



	wtv::RefPtr<wtv::IDevice> m_device;
	wtv::RefPtr<wtv::IWindow> m_window;
	wtv::RefPtr<wtv::IServiceProvider> m_services;
	wtv::RefPtr<wtv::IDescriptorPool> m_descPool;
	glm::uvec2 m_windowSize;
	glm::uvec2 m_windowPos;


	wtv::RefPtr<wtv::IGPURenderPass> m_imguiRenderPass;
	wtv::IFramebuffer::Properties m_framebufferInfo{};

};