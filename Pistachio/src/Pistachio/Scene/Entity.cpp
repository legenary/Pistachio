#include "ptcpch.h"
#include "Entity.h"

namespace Pistachio {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene) {}

	bool Entity::PhysicsEnabled() const {
		if (HasComponent<QuadRendererComponent>() && GetComponent<QuadRendererComponent>().Physics)
			return true;
		if (HasComponent<CircleRendererComponent>() && GetComponent<CircleRendererComponent>().Physics)
			return true;
		return false;
	}

}