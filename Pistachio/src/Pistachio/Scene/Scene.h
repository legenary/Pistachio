#pragma once

#include <entt.hpp>
#include "Pistachio/Core/Timestep.h"

namespace Pistachio {

	class Entity;
	class Scene {

	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		void OnUpdate(Timestep ts);

		// temp
		entt::registry& Reg() { return m_Registry; }

	private:
		entt::registry m_Registry;	// registry per scene

		friend class Entity;

	};

}