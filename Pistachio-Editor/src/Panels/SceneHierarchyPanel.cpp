#include "SceneHierarchyPanel.h"
#include "Pistachio/Scene/Component.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Pistachio {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene) {
		SetContext(scene);
	}
	 
	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene) {
		m_Context = scene;
	}

	void SceneHierarchyPanel::OnImGuiRender() {
		PTC_PROFILE_FUNCTION("Scene Hierarchy Panel Render");
		ImGui::Begin("Scene Hierarchy");
		m_Context->m_Registry.each([=](auto entityID) {
			Entity entity{ entityID, m_Context.get() };
			DrawEntityNode(entity);
		});
		if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(0)) {
			m_SelectionContext = {};
		}
		ImGui::End();


		ImGui::Begin("Properties");
		if (m_SelectionContext) {
			DrawComponents(m_SelectionContext);
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity) {
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = 
			((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) 
			| ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked()) {
			m_SelectionContext = entity;
			//PTC_CORE_INFO("selected entity name: {0}", entity.GetComponent<TagComponent>().Tag);
		}
		if (opened) {
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity) {

		DrawComponent<TagComponent>("Tag", entity, [&]() {
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer, tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
				tag = std::string(buffer);
			}
		});

		DrawComponent<TransformComponent>("Position", entity, [&]() {
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(),
				ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
				auto& transform = entity.GetComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.5f);
				ImGui::TreePop();
			}
		});

		DrawComponent<CameraComponent>("Camera", entity, [&]() {
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(),
				ImGuiTreeNodeFlags_DefaultOpen, "Camera")) {
				auto& cameraComp = entity.GetComponent<CameraComponent>();
				auto& camera = cameraComp.Camera;
				const char* projectiontypeStrings[] = { "perspective", "orthographic" };
				const char* currentProjectionTypeString = projectiontypeStrings[(int)(camera.GetProjectionType())];

				ImGui::BeginDisabled();
				ImGui::Checkbox("Primary", &cameraComp.Primary);
				ImGui::EndDisabled();

				if (ImGui::BeginCombo("Type", currentProjectionTypeString)) {
					for (int i = 0; i < 2; i++) {
						bool isSelected = currentProjectionTypeString == projectiontypeStrings[i];
						if (ImGui::Selectable(projectiontypeStrings[i], isSelected)) {
							currentProjectionTypeString = projectiontypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				switch (camera.GetProjectionType()) {
				case SceneCamera::ProjectionType::Perspective: {
					float perspectiveFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("FOV", &perspectiveFOV))
						camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveFOV));

					float perspectiveNearClip = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near Clip", &perspectiveNearClip))
						camera.SetPerspectiveNearClip(perspectiveNearClip);

					float perspectiveFarClip = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far Clip", &perspectiveFarClip))
						camera.SetPerspectiveFarClip(perspectiveFarClip);

					break;
				}
				case SceneCamera::ProjectionType::Orthographic: {
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float orthoNearClip = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near Clip", &orthoNearClip))
						camera.SetOrthographicNearClip(orthoNearClip);

					float orthoFarClip = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far Clip", &orthoFarClip))
						camera.SetOrthographicFarClip(orthoFarClip);

					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComp.FixedAspectRatio);
					break;
				}
				}

				ImGui::TreePop();
			}
		});

		DrawComponent<SpriteRendererComponent>("Sprite", entity, [&]() {
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(),
				ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer")) {
				auto& spriteComp = entity.GetComponent<SpriteRendererComponent>();
				ImGui::ColorEdit4("Color", glm::value_ptr(spriteComp.Color));

				ImGui::TreePop();
			}
		});
	}
}