#include "ptcpch.h"
#include "World.h"

namespace Pistachio {

	World::World() {
	}

	World::~World() {
	}

	void World::Init() {
		m_Gravity = b2Vec2(0.0f, -9.8f);
		m_PhysicsWorld.reset(new b2World(m_Gravity));
	}

	void World::addBoxComponent(const glm::vec2& position, float rotation, const glm::vec2& dimensions, bool dynamic) {
		m_ShapeComponents.push_back(new BoxComponent(m_PhysicsWorld.get(), position, rotation, dimensions, dynamic));
	}

	void World::addCircleComponent(const glm::vec2& position, float radius, bool dynamic) {
		m_ShapeComponents.push_back(new CircleComponent(m_PhysicsWorld.get(), position, radius, dynamic));
	}
	


}
