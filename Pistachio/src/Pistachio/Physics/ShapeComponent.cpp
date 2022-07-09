#include "ptcpch.h"
#include "ShapeComponent.h"

#include "Pistachio/Renderer/Renderer2D.h"

namespace Pistachio {

	ShapeComponent::ShapeComponent(b2World* world, const glm::vec2& position, float rotation, bool dynamic) {
		// make body
		b2BodyDef bodyDef;
		bodyDef.type = (dynamic) ? b2_dynamicBody : b2_staticBody;
		bodyDef.position.Set(position.x, position.y);
		bodyDef.angle = glm::radians(rotation);
		bodyDef.fixedRotation = false;
		m_Body = world->CreateBody(&bodyDef);

	}

	BoxComponent::BoxComponent(b2World* world, const glm::vec2& position, float rotation, const glm::vec2& dimensions, bool dynamic)
		: ShapeComponent(world, position, rotation, dynamic), m_Width(dimensions.x), m_Height(dimensions.y) {
		b2PolygonShape boxShape;
		boxShape.SetAsBox(m_Width / 2.0f, m_Height / 2.0f);
		m_Type = type::box;

		// make fixture
		b2FixtureDef fixtureDef;
		fixtureDef.density = (dynamic) ? 1.0f : 0.0f;
		fixtureDef.friction = 0.3f;
		fixtureDef.restitution = 0.0f;
		fixtureDef.restitutionThreshold = 0.5f;
		fixtureDef.shape = &boxShape;
		m_Fixture = m_Body->CreateFixture(&fixtureDef);
	}

	void BoxComponent::Draw(const glm::vec4& color) const {
		Pistachio::Renderer2D::DrawQuad(color,
			getPosition(), glm::vec2(m_Width, m_Height), getRotation());
	}

	CircleComponent::CircleComponent(b2World* world, const glm::vec2& position, float radius, bool dynamic)
		: ShapeComponent(world, position, 0, dynamic), m_Radius(radius) {
		b2CircleShape circleShape;
		circleShape.m_radius = m_Radius;
		m_Type = type::circle;

		// make fixture
		b2FixtureDef fixtureDef;
		fixtureDef.density = (dynamic) ? 1.0f : 0.0f;
		fixtureDef.friction = 0.3f;
		fixtureDef.restitution = 0.0f;
		fixtureDef.restitutionThreshold = 0.5f;
		fixtureDef.shape = &circleShape;
		m_Fixture = m_Body->CreateFixture(&fixtureDef);
	}

	void CircleComponent::Draw(const glm::vec4& color) const {
		Pistachio::Renderer2D::DrawCircle(color,
			getPosition(), m_Radius, getRotation());
	}

}