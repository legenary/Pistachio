#pragma once

#include "Pistachio.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Pistachio/Renderer/EditorCamera.h"

namespace Pistachio {

	class EditorLayer : public Layer {
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;


	private:
		void ImGuiPrep();
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		void SceneSave();
		void SceneSaveAs();
		void SceneNew();
		void SceneOpen();

		void UI_Toolbar();
		void OnScenePlay() { m_SceneState = SceneState::Play; }
		void OnSceneStop() { m_SceneState = SceneState::Edit; }

	private:
		glm::vec2 m_ViewportSize;
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		glm::vec2 m_ViewportBounds[2];
	
		Ref<Texture2D> m_Texture, m_EmojiTexture, m_SpriteSheet;
		OrthographicCameraController m_CameraController;
		
		Ref<Scene> m_ActiveScene;
		Entity m_HoveredEntity;
		EditorCamera m_EditorCamera;
		Ref<FrameBuffer> m_Framebuffer;

		std::unordered_map<char, Ref<SubTexture2D>> m_TextureMap;

		SceneHierarchyPanel m_SceneHierarchyPanel;
		std::string m_cachedSavePath;

		int m_GizmoType;

		enum class SceneState {
			Edit = 0, Play = 1
		};
		SceneState m_SceneState = SceneState::Edit;
		Ref<Texture2D> m_IconPlay, m_IconStop;
	};

	

}


