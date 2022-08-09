#pragma once

#include <entt.hpp>
#include "Pistachio/Core/Timestep.h"

#include "Pistachio/Renderer/EditorCamera.h"

class b2World;

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
		void OnStartRuntime();
		void OnStopRuntime();
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCameraEntity();

		void Clear() { m_Registry.clear(); }

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
		template<typename T>
		void OnComponentRemoved(Entity entity);


	private:
		Camera* runTimeMainCamera = nullptr;
		int32_t m_ViewportWidth = 1, m_ViewportHeight = 1;

		entt::registry m_Registry;	// registry per scene
		
		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;

		// physics
		b2World* m_PhysicsWorld = nullptr;
	};

}