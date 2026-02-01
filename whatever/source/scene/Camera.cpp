#pragma once
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
namespace wtv
{
	Camera::Camera(const CreationParams& params)
	{
		m_viewMatrix = glm::lookAtRH(params.position, params.position + params.normalizedDirection, params.up);

		float fovYInRadians = glm::radians(params.fovYInDegrees);
		m_projectionMatrix = glm::perspective(fovYInRadians, params.widthToHeightRatio, params.near, params.far);
	}
}