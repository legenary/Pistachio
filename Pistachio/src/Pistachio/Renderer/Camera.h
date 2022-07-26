#pragma once

#include <glm/glm.hpp>

namespace Pistachio {

	class Camera {

	public:
		Camera() = default;
		Camera(const glm::mat4& projection)
			: m_ProjectionMatrix(projection) {}
		virtual ~Camera() = default;


		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

		// TODO:

	protected:

		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
	};

}