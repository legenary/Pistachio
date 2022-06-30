#include "ptcpch.h"
#include "Box.h"

#include "Pistachio/Renderer/Renderer2D.h"

namespace Pistachio {

	Box::Box(b2World* world, const glm::vec2& position, float rotation, const glm::vec2& dimensions, bool dynamic) {
		m_Width = dimensions.x;
		m_Height = dimensions.y;
		// make body
		b2BodyDef bodyDef;
		bodyDef.type = (dynamic) ? b2_dynamicBody : b2_staticBody;
		bodyDef.position.Set(position.x, position.y);
		bodyDef.angle = rotation;
		bodyDef.fixedRotation = false;
		m_Body = world->CreateBody(&bodyDef);

		// make fixture
		b2PolygonShape boxShape;
		boxShape.SetAsBox(m_Width/2.0f, m_Height/2.0f);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &boxShape;
		fixtureDef.density = (dynamic) ? 1.0f : 0.0f;
		fixtureDef.friction = 0.3f;
		fixtureDef.restitution = 0.0f;
		fixtureDef.restitutionThreshold = 0.5f;
		m_Fixture = m_Body->CreateFixture(&fixtureDef);
	}

	Box::~Box() {
	}


	void Box::Draw(const glm::vec4& color) const {
		Pistachio::Renderer2D::DrawQuad(color, 
			getPosition(), getDimension(), getRotation());

	}
}
