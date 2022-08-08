#pragma once

#include <entt.hpp>
#include "Pistachio/Core/Timestep.h"

#include "Pistachio/Renderer/EditorCamera.h"

namespace Pistachio {

	class Camera;
	class Entity;
	class Scene {

	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnUpdateRuntime(Timestep ts, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCameraEntity();

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		Camera* runTimeMainCamera = nullptr;
		int32_t m_ViewportWidth = 1, m_ViewportHeight = 1;

		entt::registry m_Registry;	// registry per scene
		
		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};

}