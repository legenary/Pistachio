#pragma once

#include "entt.hpp"
#include "Pistachio/Core/Timestep.h"

namespace Pistachio {

	class Scene {

	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		void OnUpdate(Timestep ts);

		// temp
		entt::registry& Reg() { return m_Registry; }

	private:
		entt::registry m_Registry;	// registry per scene


	};

}