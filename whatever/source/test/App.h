#pragma once
#include "graphics/IDevice.h"
#include "ui/IWindow.h"
#include "IServiceProvider.h"
#include "util/RefPtr.h"
#include "graphics/IFramebuffer.h"
#include "scene/Camera.h"
#include "Input.h"
#include "CameraControler.h"
using namespace wtv;



class App
{
public:
	App();
	void Run();
private:
	void Init();
	void ProcessInput(float dt);


	RefPtr<IDevice> m_device;
	RefPtr<IWindow> m_window;
	RefPtr<wtv::IServiceProvider> m_services;
	RefPtr<IDescriptorPool> m_descPool;
	RefPtr<Input> m_input;
	RefPtr<CameraController> m_cameraController;
	glm::uvec2 m_windowSize;
	glm::uvec2 m_windowPos;
	RefPtr<Camera> m_camera;

	RefPtr<IGPURenderPass> m_imguiRenderPass;
	IFramebuffer::Properties m_framebufferInfo{};

};