#pragma once
#include <glm/glm.hpp>
namespace wtv
{
	class Camera
	{
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;
	public:
		struct CameraCBData
		{
			glm::mat4 viewMatrix;
			glm::mat4 projectionMatrix;
			glm::mat4 viewProjMatrix;
		};
		struct CreationParams
		{
			glm::vec3 position;
			glm::vec3 normalizedDirection;
			glm::vec3 up;
			float fovYInDegrees;
			float widthToHeightRatio;
			float near;
			float far;
		};
		Camera(const CreationParams& params);

		glm::mat4 GetViewMatrix() { return m_viewMatrix; }
		glm::mat4 GetProjectionMatrix() { return m_projectionMatrix; }
		glm::mat4 GetViewProjectionMatrix() { return m_projectionMatrix * m_viewMatrix; }

		CameraCBData GetConstantData()
		{
			CameraCBData data;
			data.viewMatrix = m_viewMatrix;
			data.projectionMatrix = m_projectionMatrix;
			data.viewProjMatrix = GetViewProjectionMatrix();
			return data;
		}
	};
}