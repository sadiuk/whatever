#pragma once
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
namespace wtv
{
	Camera::Camera(const CreationParams& params)
	{
		m_viewMatrix = glm::lookAtRH(params.position, params.position + params.normalizedDirection, params.up);

		float fovYInRadians = glm::radians(params.fovYInDegrees);
		m_projectionMatrix = glm::perspective(fovYInRadians, params.widthToHeightRatio, params.nearPlane, params.farPlane);
	}

	glm::vec3 Camera::GetPosition() const
	{
		auto translation = glm::vec3(m_viewMatrix[3][0], m_viewMatrix[3][1], m_viewMatrix[3][2]);
		return -glm::transpose(glm::mat3(m_viewMatrix)) * translation;
	}

	glm::vec3 Camera::GetForwardVector() const
	{
		return -glm::normalize(glm::vec3(m_viewMatrix[0][2], m_viewMatrix[1][2], m_viewMatrix[2][2]));
	}

	glm::vec3 Camera::GetUpVector() const
	{
		return glm::normalize(glm::vec3(m_viewMatrix[0][1], m_viewMatrix[1][1], m_viewMatrix[2][1]));
	}

	void Camera::LookAt(const glm::vec3& position, const glm::vec3& normalizedDir, const glm::vec3& up)
	{
		m_viewMatrix = glm::lookAtRH(position, position + normalizedDir, up);
	}
}