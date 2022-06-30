#pragma once

#include "box2d/box2d.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Pistachio {
	class Box {
	public:
		Box(b2World* world, const glm::vec2& position, float rotation, const glm::vec2& dimensions, bool dynamic = true);
		~Box();

		inline b2Body* getBody() const { return m_Body; }
		inline b2Fixture* getFixture() const { return m_Fixture; }
		inline glm::vec2 getPosition() const { return glm::vec2(m_Body->GetPosition().x, m_Body->GetPosition().y); }
		inline float getRotation() const { return m_Body->GetAngle(); }
		inline float getWidth() const { return m_Width; }
		inline float getHeight() const { return m_Height; }
		inline glm::vec2 getDimension() const { return glm::vec2(m_Width, m_Height); }

		void Draw(const glm::vec4& color) const;

	private:
		float m_Width, m_Height;
		b2Body* m_Body = nullptr;
		b2Fixture* m_Fixture = nullptr;
	};
}



