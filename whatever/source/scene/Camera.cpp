#pragma once
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
namespace wtv
{
	Camera::Camera(const CreationParams& params):
		m_fovYInDegrees(params.fovYInDegrees),
		m_cameraNear(params.nearPlane),
		m_cameraFar(params.farPlane)
	{
		m_viewMatrix = glm::lookAtRH(params.position, params.position + params.normalizedDirection, params.up);

		float fovYInRadians = glm::radians(params.fovYInDegrees);
		m_projectionMatrix = glm::perspectiveRH_ZO(fovYInRadians, params.widthToHeightRatio, params.nearPlane, params.farPlane);
	}

	glm::mat4 Camera::InfiniteReversePerspective(float fovY, float aspect, float zNear)
	{
		float f = 1.0f / std::tan(fovY * 0.5f);
		return glm::mat4(
			f / aspect, 0, 0, 0,
			0, f, 0, 0,
			0, 0, 0, -1,
			0, 0, zNear, 0
		);
	}

	glm::vec3 Camera::GetPosition() const
	{
		auto translation = glm::vec3(m_viewMatrix[3][0], m_viewMatrix[3][1], m_viewMatrix[3][2]);
		return -glm::transpose(glm::mat3(m_viewMatrix)) * translation;
	}

	glm::vec3 Camera::GetDirection() const
	{
		return -glm::normalize(glm::vec3(m_viewMatrix[0][2], m_viewMatrix[1][2], m_viewMatrix[2][2]));
	}

	glm::vec3 Camera::GetUpVector() const
	{
		return glm::normalize(glm::vec3(m_viewMatrix[0][1], m_viewMatrix[1][1], m_viewMatrix[2][1]));
	}

	void Camera::SetProjectionParams(float nearPlane, float farPlane, float fovYInDegrees)
	{
		m_cameraNear = nearPlane;
		m_cameraFar = farPlane;
		m_fovYInDegrees = fovYInDegrees;

		float fovYInRadians = glm::radians(m_fovYInDegrees);
		m_projectionMatrix = glm::perspectiveRH_ZO(fovYInRadians, 1.0f, m_cameraNear, m_cameraFar);
	}

	void Camera::LookAt(const glm::vec3& position, const glm::vec3& normalizedDir, const glm::vec3& up)
	{
		m_viewMatrix = glm::lookAtRH(position, position + normalizedDir, up);
	}
}