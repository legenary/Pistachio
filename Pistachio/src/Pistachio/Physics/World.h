#pragma once

#include "box2d/box2d.h"
#include "glm/glm.hpp"

#include "Pistachio/Physics/ShapeComponent.h"

namespace Pistachio {

	class World {
	public:
		World();
		~World();

		void Init();

		//void addGround(float height);
		void addBoxComponent(const glm::vec2& position, float rotation, const glm::vec2& dimensions, bool dynamic = true);
		void addCircleComponent(const glm::vec2& position, float radius, bool dynamic = true);
		
		inline b2World* getPhysicsWorld() const { return m_PhysicsWorld.get(); }

		ShapeComponent* getComponentByIndex(int idx) const { return m_ShapeComponents[idx]; }

	private:
		std::vector<ShapeComponent*> m_ShapeComponents;
		b2Vec2 m_Gravity;
		std::unique_ptr<b2World> m_PhysicsWorld;


	};

}



