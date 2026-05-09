#pragma once
#include "glm/glm.hpp"

struct AABB
{
	glm::vec3 min;
	glm::vec3 max;

	AABB(const glm::vec3& min, const glm::vec3& max)
		: min(min), max(max) {
	}


};