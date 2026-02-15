#pragma once
#include "IServiceProvider.h"
#include "scene/Camera.h"
#include "util/RefPtr.h"
#include "Input.h"
namespace wtv
{
	class CameraController : public IReferenceCounted
	{
		IServiceProvider* m_services;
		RefPtr<Input> m_input;
		RefPtr<Camera> m_camera;
		float m_yaw = 0, m_pitch = 0;
		const glm::vec3 m_initialDirection;
	public:
		CameraController(IServiceProvider* services, const RefPtr<Input>& input, const RefPtr<Camera> camera) : 
			m_input(input), 
			m_camera(camera),
			m_initialDirection(-1, 0, 0),
			m_services(services)
		{
		}
		void Update(float dt);
	};
}