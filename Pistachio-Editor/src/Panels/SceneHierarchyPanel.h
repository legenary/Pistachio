#pragma once

#include "Pistachio.h"

namespace Pistachio {

	class SceneHierarchyPanel {
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Entity& entity);

	private:
		void DrawEntityNode(Entity& entity);
		void DrawComponents(Entity& entity);

		template<typename T, typename F>
		bool DrawComponent(const std::string& name, Entity& entity, F&& func);

	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;

	};


}