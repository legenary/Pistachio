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
			DrawComponent(m_SelectionContext);
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
			PTC_CORE_INFO("selected entity name: {0}", entity.GetComponent<TagComponent>().Tag);
		}
		if (opened) {
			ImGui::TreePop();
		}

	}

	void SceneHierarchyPanel::DrawComponent(Entity entity) {
		if (entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer, tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(),
				ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
				auto& transform = entity.GetComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.5f);
				ImGui::TreePop();
			}

		}


	}
}