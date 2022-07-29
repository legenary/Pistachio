#pragma once

#include <entt.hpp>
#include "Pistachio/Core/Timestep.h"

namespace Pistachio {

	class Camera;
	class Entity;
	class Scene {

	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

		// temp
		entt::registry& Reg() { return m_Registry; }

	private:
		Camera* mainCamera = nullptr;
		int32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		entt::registry m_Registry;	// registry per scene

		friend class Entity;
		friend class SceneHierarchyPanel;
	};

}