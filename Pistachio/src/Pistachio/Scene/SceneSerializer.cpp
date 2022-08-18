#include "ptcpch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Component.h"

namespace YAML {

	template<>
	struct convert<glm::vec2> {
		static Node encode(const glm::vec2& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs) {
			if (!node.IsSequence() || node.size() != 2)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
	
	Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}
}

namespace Pistachio {

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		:m_Scene(scene) {

	}

	static void SerializeEntity(YAML::Emitter& out, Entity& entity) {
		PTC_CORE_ASSERT(entity.HasComponent<IDComponent>(), "Entity don't have a UUID");

		out << YAML::BeginMap;	// Entity
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID(); // TODO: Entity ID UUID

		if (entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;	// Tag Component
			out << YAML::Key << "Tag" << YAML::Value << tag;
			out << YAML::EndMap;	// Tag Component
		}

		if (entity.HasComponent<TransformComponent>()) {
			auto& transComp = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;	// Transform Component
			out << YAML::Key << "Translation" << YAML::Value << transComp.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << transComp.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << transComp.Scale;
			out << YAML::EndMap;	// Transform Component
		}

		if (entity.HasComponent<CameraComponent>()) {
			auto& camComp = entity.GetComponent<CameraComponent>();
			auto& cam = camComp.Camera;

			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;	// Camera Component
			out << YAML::Key << "Primary" << YAML::Value << camComp.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << camComp.FixedAspectRatio;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;	// Camera
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)cam.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << cam.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << cam.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << cam.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << cam.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << cam.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << cam.GetOrthographicFarClip();
			out << YAML::EndMap;	// Camera

			out << YAML::EndMap;	// Camera Component
		}

		if (entity.HasComponent<SpriteRendererComponent>()) {
			auto& srComp = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;	// Sprite Renderer Component
			out << YAML::Key << "Color" << YAML::Value << srComp.Color;
			out << YAML::Key << "Physics" << YAML::Value << srComp.Physics;
			out << YAML::EndMap;	// Sprite Renderer Component
		}

		if (entity.HasComponent<RigidBody2DComponent>()) {
			auto& rb2dComp = entity.GetComponent<RigidBody2DComponent>();
			out << YAML::Key << "RigidBody2DComponent";
			out << YAML::BeginMap;	// Rigid Body 2D Component
			out << YAML::Key << "Type" << YAML::Value << (int)rb2dComp.Type;
			out << YAML::Key << "FixedRotation" << YAML::Value << rb2dComp.FixedRotation;
			out << YAML::EndMap;	// Rigid Body 2D Component
		}

		if (entity.HasComponent<BoxCollider2DComponent>()) {
			auto& bc2dComp = entity.GetComponent<BoxCollider2DComponent>();
			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap;	// Box Collider 2D Component
			out << YAML::Key << "OffsetPos" << YAML::Value << bc2dComp.OffsetPos;
			out << YAML::Key << "OffsetAngle" << YAML::Value << bc2dComp.OffsetAngle;
			out << YAML::Key << "Size" << YAML::Value << bc2dComp.Size;
			out << YAML::Key << "Density" << YAML::Value << bc2dComp.Density;
			out << YAML::Key << "Friction" << YAML::Value << bc2dComp.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << bc2dComp.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << bc2dComp.RestitutionThreshold;
			
			out << YAML::EndMap;	// Box Collider 2D Component
		}

		if (entity.HasComponent<CircleRendererComponent>()) {
			auto& crComp = entity.GetComponent<CircleRendererComponent>();
			out << YAML::Key << "CircleRendererComponent";
			out << YAML::BeginMap;	// Circle Renderer Component
			out << YAML::Key << "Color" << YAML::Value << crComp.Color;
			out << YAML::Key << "Thickness" << YAML::Value << crComp.Thickness;
			out << YAML::Key << "Fade" << YAML::Value << crComp.Fade;
			out << YAML::Key << "Physics" << YAML::Value << crComp.Physics;
			out << YAML::EndMap;	// Circle Renderer Component
		}

		if (entity.HasComponent<CircleCollider2DComponent>()) {
			auto& cc2dComp = entity.GetComponent<CircleCollider2DComponent>();
			out << YAML::Key << "CircleCollider2DComponent";
			out << YAML::BeginMap;	// Circle Collider 2D Component
			out << YAML::Key << "OffsetPos" << YAML::Value << cc2dComp.OffsetPos;
			out << YAML::Key << "Radius" << YAML::Value << cc2dComp.Radius;
			out << YAML::Key << "Density" << YAML::Value << cc2dComp.Density;
			out << YAML::Key << "Friction" << YAML::Value << cc2dComp.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << cc2dComp.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << cc2dComp.RestitutionThreshold;

			out << YAML::EndMap;	// Circle Collider 2D Component
		}

		out << YAML::EndMap;	// Entity
	}

	void SceneSerializer::Serialize(const std::string& filepath) {
		YAML::Emitter out;
		out << YAML::BeginMap;
		std::string name = "Untitled";
		out << YAML::Key << "Scene" << YAML::Value << name;
		PTC_CORE_TRACE("Serializing scene '{0}'...", name);
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID) {
			Entity entity = { entityID, m_Scene.get() };
			if (!entity) {
				return;
			}
			SerializeEntity(out, entity);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
		PTC_CORE_TRACE("Serializing scene '{0}' success!", name);
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath) {
		PTC_CORE_ASSERT(false, "Not implemented");
	}

	bool SceneSerializer::Deserialize(const std::string& filepath) {
		std::fstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		PTC_CORE_TRACE("Deserializing scene '{0}'...", sceneName);

		YAML::Node entities = data["Entities"];
		if (entities) {
			for (auto entity : entities) {
				m_EntityNodes.push(entity);
			}
			while(!m_EntityNodes.empty()) {
				auto entity = m_EntityNodes.top();
				m_EntityNodes.pop();

				uint64_t id = entity["Entity"].as<uint64_t>();
				
				auto tagComp = entity["TagComponent"];
				std::string name = tagComp["Tag"].as<std::string>();
				//PTC_CORE_TRACE("Deserializing entity... {0}", name);
				Entity deserializedEntity = m_Scene->CreateEntityWithUUID(UUID(id), name);

				auto transComp = entity["TransformComponent"];
				if (transComp) {
					auto& comp = deserializedEntity.GetComponent<TransformComponent>();
					comp.Translation = transComp["Translation"].as<glm::vec3>();
					comp.Rotation = transComp["Rotation"].as<glm::vec3>();
					comp.Scale = transComp["Scale"].as<glm::vec3>();
				}

				auto camComp = entity["CameraComponent"];
				if (camComp) {
					auto& comp = deserializedEntity.AddComponent<CameraComponent>();
					comp.Primary = camComp["Primary"].as<bool>();
					comp.FixedAspectRatio = camComp["FixedAspectRatio"].as<bool>();

					auto& cameraProps = camComp["Camera"];
					comp.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());
					comp.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
					comp.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					comp.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());
					comp.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					comp.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					comp.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());
				}

				auto spriteComp = entity["SpriteRendererComponent"];
				if (spriteComp) {
					auto& comp = deserializedEntity.AddComponent<SpriteRendererComponent>();
					comp.Color = spriteComp["Color"].as<glm::vec4>();
					comp.Physics = spriteComp["Physics"].as<bool>();
				}

				auto rb2dComp = entity["RigidBody2DComponent"];
				if (rb2dComp) {
					auto& comp = deserializedEntity.AddComponent<RigidBody2DComponent>();
					comp.Type = (RigidBody2DComponent::BodyType)(rb2dComp["Type"].as<int>());
					comp.FixedRotation = rb2dComp["FixedRotation"].as<bool>();
				}

				auto bc2dComp = entity["BoxCollider2DComponent"];
				if (bc2dComp) {
					auto& comp = deserializedEntity.AddComponent<BoxCollider2DComponent>();
					comp.OffsetPos = bc2dComp["OffsetPos"].as<glm::vec2>();
					comp.OffsetAngle = bc2dComp["OffsetAngle"].as<float>();
					comp.Size = bc2dComp["Size"].as<glm::vec2>();
					comp.Density = bc2dComp["Density"].as<float>();
					comp.Friction = bc2dComp["Friction"].as<float>();
					comp.Restitution = bc2dComp["Restitution"].as<float>();
					comp.RestitutionThreshold = bc2dComp["RestitutionThreshold"].as<float>();
				}

				auto circleComp = entity["CircleRendererComponent"];
				if (circleComp) {
					auto& comp = deserializedEntity.AddComponent<CircleRendererComponent>();
					comp.Color = circleComp["Color"].as<glm::vec4>();
					comp.Thickness = circleComp["Thickness"].as<float>();
					comp.Fade = circleComp["Fade"].as<float>();
					comp.Physics = circleComp["Physics"].as<bool>();
				}

				auto cc2dComp = entity["CircleCollider2DComponent"];
				if (cc2dComp) {
					auto& comp = deserializedEntity.AddComponent<CircleCollider2DComponent>();
					comp.OffsetPos = cc2dComp["OffsetPos"].as<glm::vec2>();
					comp.Radius = cc2dComp["Radius"].as<float>();
					comp.Density = cc2dComp["Density"].as<float>();
					comp.Friction = cc2dComp["Friction"].as<float>();
					comp.Restitution = cc2dComp["Restitution"].as<float>();
					comp.RestitutionThreshold = cc2dComp["RestitutionThreshold"].as<float>();
				}
			}
		}

		PTC_CORE_TRACE("Deserializing scene '{0}' success!", sceneName);
		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath) {
		PTC_CORE_ASSERT(false, "Not implemented");
		return false;
	}
}