#include "ptcpch.h"
#include "Physics.h"

namespace Pistachio {

	Physics::Physics() {
	}

	Physics::~Physics() {
		delete m_PysicsWorld;
		m_PysicsWorld = nullptr;
	}

	void Physics::Init() {
		m_Gravity = b2Vec2(0.0f, -9.8f);
		m_PysicsWorld = new b2World(m_Gravity);
	}

	//void Physics::addGround(float height) {
	//	// body def
	//	b2BodyDef groundBodyDef;
	//	groundBodyDef.position.Set(0.0f, height-0.1f);
	//	b2Body* groundBody = m_PysicsWorld->CreateBody(&groundBodyDef);
	//	// fixture
	//	b2PolygonShape groundBox;
	//	groundBox.SetAsBox(200.0f, 1.0f);
	//	groundBody->CreateFixture(&groundBox, 0.0f /*static*/);

	//}

	void Physics::addBox(const glm::vec2& position, float rotation, const glm::vec2& dimensions, bool dynamic) {
		m_Boxes.push_back(Box(m_PysicsWorld, position, rotation, dimensions, dynamic));
	}
	


}
