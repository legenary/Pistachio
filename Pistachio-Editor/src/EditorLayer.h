#pragma once

#include "Pistachio.h"

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
		glm::vec2 m_ViewportSize;
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
	
		Ref<Texture2D> m_Texture, m_EmojiTexture, m_SpriteSheet;
		OrthographicCameraController m_CameraController;
		
		Ref<Scene> m_ActiveScene;
		entt::entity m_SquareEntity;
		Ref<FrameBuffer> m_Framebuffer;

		std::unordered_map<char, Ref<SubTexture2D>> m_TextureMap;
	};

}


