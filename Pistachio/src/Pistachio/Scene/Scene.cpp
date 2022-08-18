#include "ptcpch.h"
#include "Scene.h"
#include "Component.h"

#include "Pistachio/Renderer/Renderer.h"
#include "Pistachio/Scene/Entity.h"
#include "Pistachio.h"
#include <glm/glm.hpp>

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>

namespace Pistachio {

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

	// create entity and generate a randomn UUID
	Entity Scene::CreateEntity(const std::string& name) {
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID& id, const std::string& name) {
		Entity e = { m_Registry.create(), this };
		e.AddComponent<IDComponent>(id);
		e.AddComponent<TransformComponent>();
		auto& tag = e.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Unnamed Entity" : name;
		return e;
	}

	void Scene::DestroyEntity(Entity entity) {
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& editorCamera) {
		Renderer2D::BeginScene(editorCamera);

		// retrive entities that has multiple components
		auto group1 = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group1) {
			auto [trans, sprite] = group1.get<TransformComponent, SpriteRendererComponent>(entity);
			//Renderer2D::DrawQuad(sprite.Color, trans.GetTransform());
			Renderer2D::DrawSpriteRenderer(sprite, trans.GetTransform(), (int)(uint32_t)(entity));
		}
		// retrive entities that has multiple components
		auto group2 = m_Registry.view<TransformComponent, CircleRendererComponent>();
		for (auto entity : group2) {
			auto [trans, circle] = group2.get<TransformComponent, CircleRendererComponent>(entity);
			//Renderer2D::DrawQuad(sprite.Color, trans.GetTransform());
			Renderer2D::DrawCircleRenderer(circle, trans.GetTransform(), (int)(uint32_t)(entity));
		}
		Renderer2D::EndScene();
	}

	void Scene::OnStartRuntime() {
		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });
		auto view = m_Registry.view<RigidBody2DComponent>();
		for (auto e : view) {
			Entity entity = { e, this };
			if (!entity.PhysicsEnbaled())
				continue;

			auto& transformComp = entity.GetComponent<TransformComponent>();
			auto& rb2dComp = entity.GetComponent<RigidBody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = (b2BodyType)rb2dComp.Type;
			bodyDef.position.Set(transformComp.Translation.x, transformComp.Translation.y);
			bodyDef.angle = transformComp.Rotation.z;
			bodyDef.fixedRotation = rb2dComp.FixedRotation;
			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			rb2dComp.RuntimeBody = body;

			if (entity.HasComponent<BoxCollider2DComponent>()) {
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
				b2PolygonShape boxShape;
				boxShape.SetAsBox(transformComp.Scale.x * bc2d.Size.x, transformComp.Scale.y * bc2d.Size.y,
					{bc2d.OffsetPos.x, bc2d.OffsetPos.y}, bc2d.OffsetAngle);
			
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.Density;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
			else if (entity.HasComponent<CircleCollider2DComponent>()) {
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
				b2CircleShape circleShape;
				circleShape.m_radius = transformComp.Scale.x * cc2d.Radius;
				circleShape.m_p.Set(cc2d.OffsetPos.x, cc2d.OffsetPos.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = cc2d.Density;
				fixtureDef.friction = cc2d.Friction;
				fixtureDef.restitution = cc2d.Restitution;
				fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::OnStopRuntime() {
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	void Scene::OnUpdateRuntime(Timestep ts, EditorCamera& editorCamera) {

		// Update scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
				// TODO: move to scene::OnScenePlay
				if (!nsc.Instance) {
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				}
				nsc.Instance->OnUpdate(ts);
			});
			// TODO: add nsc.Instance->OnDestroy to scene::OnSceneStop
		}

		// Update physics
		{
			const uint32_t velocityIterations = 6;
			const uint32_t positionIterations = 6;
			m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

			auto view = m_Registry.view<RigidBody2DComponent>();
			for (auto e : view) {
				Entity entity = { e, this };
				if (entity.PhysicsEnbaled()) {
				//if (entity.HasComponent<SpriteRendererComponent>() &&
				//	entity.GetComponent<SpriteRendererComponent>().Physics) {
					auto& transComp = entity.GetComponent<TransformComponent>();
					auto& rb2dComp = entity.GetComponent<RigidBody2DComponent>();
					b2Body* body = (b2Body*)rb2dComp.RuntimeBody;
					const auto& position = body->GetPosition();
					transComp.Translation.x = position.x;
					transComp.Translation.y = position.y;
					transComp.Rotation.z = body->GetAngle();
				}
			}
		}
		

		// Update rendering data
		runTimeMainCamera = nullptr;
		glm::mat4 transform;
		auto group = m_Registry.group<CameraComponent>(entt::get<TransformComponent>);
		for (auto entity : group) {
			auto [cameraComp, transComp] = group.get<CameraComponent, TransformComponent>(entity);
			if (cameraComp.Primary) {
				runTimeMainCamera = &(cameraComp.Camera);
				transform = transComp.GetTransform();
				break;
			}
		}

		if (runTimeMainCamera) {
			Renderer2D::BeginScene(*runTimeMainCamera, transform);
			auto group1 = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group1) {
				auto [trans, sprite] = group1.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawSpriteRenderer(sprite, trans.GetTransform(), (int)(uint32_t)entity);
			}
			auto group2 = m_Registry.view<TransformComponent, CircleRendererComponent>();
			for (auto entity : group2) {
				auto [trans, sprite] = group2.get<TransformComponent, CircleRendererComponent>(entity);
				Renderer2D::DrawCircleRenderer(sprite, trans.GetTransform(), (int)(uint32_t)entity);
			}
			Renderer2D::EndScene();
		}
		else { // if no primary camera, use editor camera location
			Renderer2D::BeginScene(editorCamera);
			auto group1 = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group1) {
				auto [trans, sprite] = group1.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawSpriteRenderer(sprite, trans.GetTransform(), (int)(uint32_t)entity);
			}
			auto group2 = m_Registry.view<TransformComponent, CircleRendererComponent>();
			for (auto entity : group2) {
				auto [trans, sprite] = group2.get<TransformComponent, CircleRendererComponent>(entity);
				Renderer2D::DrawCircleRenderer(sprite, trans.GetTransform(), (int)(uint32_t)entity);
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

	Entity Scene::GetPrimaryCameraEntity() {
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary) {
				return Entity{ entity, this };
			}
		}
		return {};
	}


	// temporary
	// template specialization
	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component) {
		//static_assert(false);
	}
	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component) {}
	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component) {}
	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {}
	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component) {
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}
	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component) {}
	template<>
	void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component) {}
	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {}
	template<>
	void Scene::OnComponentAdded<RigidBody2DComponent>(Entity entity, RigidBody2DComponent& component) {}
	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component) {}
	template<>
	void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component) {}


	template<typename T>
	void Scene::OnComponentRemoved(Entity entity) {
		//static_assert(false);
	}
	template<>
	void Scene::OnComponentRemoved<TagComponent>(Entity entity) {}
	template<>
	void Scene::OnComponentRemoved<IDComponent>(Entity entity) {}
	template<>
	void Scene::OnComponentRemoved<TransformComponent>(Entity entity) {}
	template<>
	void Scene::OnComponentRemoved<CameraComponent>(Entity entityt) {}
	template<>
	void Scene::OnComponentRemoved<SpriteRendererComponent>(Entity entity) {
		entity.RemoveComponent<RigidBody2DComponent>();
		entity.RemoveComponent<BoxCollider2DComponent>();
	}
	template<>
	void Scene::OnComponentRemoved<CircleRendererComponent>(Entity entity) {
		entity.RemoveComponent<RigidBody2DComponent>();
		entity.RemoveComponent<CircleCollider2DComponent>();
	}
	template<>
	void Scene::OnComponentRemoved<NativeScriptComponent>(Entity entity) {}
	template<>
	void Scene::OnComponentRemoved<RigidBody2DComponent>(Entity entity) {}
	template<>
	void Scene::OnComponentRemoved<BoxCollider2DComponent>(Entity entity) {}
	template<>
	void Scene::OnComponentRemoved<CircleCollider2DComponent>(Entity entity) {}

}