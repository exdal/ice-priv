#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace IceSDK::Components
{
	struct TransformComponent
	{
		glm::vec3 position;
		glm::vec3 scale;

		glm::mat4 model_matrix;
	};
} // namespace IceSDK::Components
