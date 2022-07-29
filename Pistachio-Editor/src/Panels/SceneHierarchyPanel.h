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
		void DrawComponents(Entity entity);

		template<typename T, typename F>
		bool DrawComponent(Entity& entity, F&& func) {
			if (!entity.HasComponent<T>())
				return false;
			func();
			return true;
		}

	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;

	};

}