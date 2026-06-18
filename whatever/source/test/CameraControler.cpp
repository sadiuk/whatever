#include "CameraControler.h"
#include "util/ILogger.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/euler_angles.hpp"

namespace wtv
{
	void CameraController::Update(float dt)
	{
		float mouseSensitivity = 0.02;
		float step = dt * 100;
		auto position = m_camera->GetPosition();
		auto direction = glm::normalize(m_camera->GetForwardVector());
		auto right = glm::normalize(glm::cross(direction, m_camera->GetUpVector()));
		if (m_input->IsKeyDown(Key::LeftShift))
		{
			step *= 10;
		}
		if (m_input->IsKeyDown(Key::W))
		{
			position = position + direction * step;
		}
		if (m_input->IsKeyDown(Key::S))
		{
			position = position - direction * step;
		}
		if (m_input->IsKeyDown(Key::A))
		{
			position = position - right * step;
		}
		if (m_input->IsKeyDown(Key::D))
		{
			position = position + right * step;
		}
		if (m_input->IsMouseButtonDown(MouseButton::Right))
		{
			glm::ivec2 pixelDelta = m_input->GetRelativeMousePosition();
			if (pixelDelta != glm::ivec2(0))
			{
				m_yaw -= pixelDelta.x * mouseSensitivity;
				m_pitch += -pixelDelta.y * mouseSensitivity;
			/*	auto rotation = glm::yawPitchRoll(m_yaw, 0.0f, m_pitch);
				direction = rotation * glm::vec4(m_initialDirection, 0);*/
				m_pitch = glm::clamp(m_pitch, -glm::half_pi<float>(), glm::half_pi<float>());
				direction.x = cos(m_pitch) * cos(m_yaw);
				direction.y = cos(m_pitch) * sin(m_yaw);
				direction.z = sin(m_pitch);
				m_services->GetService<ILogger>()->Log(ILogger::Severity::Debug, "Pitch/yaw: {}, {}", m_pitch, m_yaw);
				//m_services->GetService<ILogger>()->Log(ILogger::Severity::Debug, "Direction: ({}, {}, {})", direction.x, direction.y, direction.z);
			}
		}
		m_camera->LookAt(position, direction, glm::vec3(0, 0, 1));
	}
}