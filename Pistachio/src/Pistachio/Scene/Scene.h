#pragma once

#include <entt.hpp>
#include "Pistachio/Core/Timestep.h"

#include "Pistachio/Renderer/EditorCamera.h"

class b2World;

namespace Pistachio {

	class Camera;
	class Entity;
	class UUID;
	class Scene {

	public:
		Scene();
		~Scene();
		//Scene& operator=(Scene other) {
		//	if (this != &other) {
		//		this->m_Registry = entt::registry();
		//		this->m_Registry.assign(other.m_Registry.data(),
		//			other.m_Registry.data() + other.m_Registry.size(),
		//			other.m_Registry.released());

		//		this->m_PhysicsWorld = nullptr;
		//		this->runTimeMainCamera = nullptr;
		//	}
		//	return *this;
		//}
		Scene(const Scene& other) {
			m_Registry = entt::registry();
			m_Registry.assign(other.m_Registry.data(),
				other.m_Registry.data() + other.m_Registry.size(),
				other.m_Registry.released());

			m_PhysicsWorld = nullptr;
			runTimeMainCamera = nullptr;
		}




		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID& id, const std::string& name = std::string());
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