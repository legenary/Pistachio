#pragma once

#include "glm/glm.hpp"
#include "box2d/box2d.h"

namespace Pistachio {
	class ShapeComponent {
	public:
		ShapeComponent(b2World* world, const glm::vec2& position, float rotation, bool dynamic = true);
		~ShapeComponent() = default;

		enum class type {
			none = 0, box, circle
		};

		inline b2Body* getBody() const { return m_Body; }
		inline b2Fixture* getFixture() const { return m_Fixture; }
		inline glm::vec2 getPosition() const { return glm::vec2(m_Body->GetPosition().x, m_Body->GetPosition().y); }
		inline float getRotation() const { return glm::degrees(m_Body->GetAngle()); }

		virtual void Draw(const glm::vec4& color) const = 0;

	protected:
		b2Body* m_Body = nullptr;
		b2Fixture* m_Fixture = nullptr;

		type m_Type;
	};

	class BoxComponent : public ShapeComponent {
	public:
		BoxComponent(b2World* world, const glm::vec2& position, float rotation, const glm::vec2& dimensions, bool dynamic = true);
		~BoxComponent() = default;
		virtual void Draw(const glm::vec4& color) const override;

	private:
		float m_Width, m_Height;
	};

	class CircleComponent : public ShapeComponent {
	public:
		CircleComponent(b2World* world, const glm::vec2& position, float radius, bool dynamic = true);
		~CircleComponent() = default;
		virtual void Draw(const glm::vec4& color) const override;

	private:
		float m_Radius;
	};



}


