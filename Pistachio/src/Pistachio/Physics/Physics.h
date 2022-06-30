#pragma once

#include "box2d/box2d.h"
#include "glm/glm.hpp"

#include "Pistachio/Objects/Box.h"

namespace Pistachio {

	class Physics {
	public:
		Physics();
		~Physics();

		void Init();

		//void addGround(float height);
		void addBox(const glm::vec2& position, float rotation, const glm::vec2& dimensions, bool dynamic = true);
		
		inline b2World* getWorld() const { return m_PysicsWorld; }

		Box getObjectByIndex(int idx) const { return m_Boxes[idx]; }

	private:
		std::vector<Box> m_Boxes;
		b2Vec2 m_Gravity;
		b2World* m_PysicsWorld = nullptr;


	};

}



