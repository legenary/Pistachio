#pragma once

#include "Pistachio.h"

namespace Pistachio {

	class SceneHierarchyPanel {
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponent(Entity entity);

	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;

	};

}