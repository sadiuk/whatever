#pragma once
#include "util/RefPtr.h"
#include <glm/glm.hpp>
namespace wtv
{
	class Camera : public IReferenceCounted
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
			float nearPlane;
			float farPlane;
		};
		Camera(const CreationParams& params);

		glm::mat4 GetViewMatrix() { return m_viewMatrix; }
		glm::mat4 GetProjectionMatrix() { return m_projectionMatrix; }
		glm::mat4 GetViewProjectionMatrix() { return m_projectionMatrix * m_viewMatrix; }
		glm::vec3 GetPosition() const;
		glm::vec3 GetForwardVector() const;
		glm::vec3 GetUpVector() const;

		void LookAt(const glm::vec3& position, const glm::vec3& normalizedDir, const glm::vec3& up);

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