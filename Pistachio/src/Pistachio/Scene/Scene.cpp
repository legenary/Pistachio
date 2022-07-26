#include "ptcpch.h"
#include "Scene.h"
#include "Component.h"

#include "Pistachio/Renderer/Renderer.h"
#include "Pistachio/Scene/Entity.h"
#include <glm/glm.hpp>

namespace Pistachio {

	//static void OnTransformConstruct() {}

	Scene::Scene() {
		//// example code

		//// create a component
		//struct MeshComponent {
		//	float value;
		//	MeshComponent() = default;
		//};
		//struct TransformComponent {
		//	glm::mat4 Transform;

		//	TransformComponent() = default;
		//	TransformComponent(const TransformComponent&) = default;
		//	TransformComponent(const glm::mat4& trans)
		//		: Transform(trans) {}

		//	operator glm::mat4& () { return Transform; }
		//	operator const glm::mat4& () const { return Transform; }
		//};

		//// add component to entity
		//entt::entity entity = m_Registry.create();
		//auto& transform = m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		//// retrieve component from entity
		//if (m_Registry.any_of<TransformComponent>(entity)) {
		//	TransformComponent& transformComp = m_Registry.get<TransformComponent>(entity);
		//}

		//// retrive entities that has a certain component
		//auto view = m_Registry.view<TransformComponent>();
		//for (auto entity : view) {
		//	auto& transformComp = view.get<TransformComponent>(entity);
		//}

		//// retrive entities that has multiple components
		//auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		//for (auto entity : group) {
		//	auto& [transformComp, meshComp] = group.get<TransformComponent, MeshComponent>(entity);
		//}

		//// event listener
		////m_Registry.on_destroy...
		////m_Registry.on_update...
		//m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();


	}

	Scene::~Scene() {
	}

	Entity Scene::CreateEntity(const std::string& name) {
		Entity e = { m_Registry.create(), this };
		e.AddComponent<TransformComponent>();
		auto& tag = e.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Unnamed Entity" : name;
		return e;
	}


	void Scene::OnUpdate(Timestep ts) {

		// Render sprites
		mainCamera = nullptr;
		glm::mat4* transform = nullptr;
		auto group = m_Registry.group<CameraComponent>(entt::get<TransformComponent>);
		for (auto entity : group) {
			auto& [cameraComp, transComp] = group.get<CameraComponent, TransformComponent>(entity);
			if (cameraComp.Primary) {
				mainCamera = &(cameraComp.Camera);
				transform = &(transComp.Transform);
				break;
			}
		}

		if (mainCamera) {

			Renderer2D::BeginScene(*mainCamera, *transform);

			// retrive entities that has multiple components
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
			for (auto entity : group) {
				auto& [trans, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
			
				Renderer2D::DrawQuad(sprite.Color, trans);

			}
			Renderer2D::EndScene();

		}

	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height) {
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize non-fixed aspect ratio camera
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			auto& cameraComp = view.get<CameraComponent>(entity);
			if (!cameraComp.FixedAspectRatio) {
				cameraComp.Camera.SetViewportSize(width, height);
			}
		}
	}

}