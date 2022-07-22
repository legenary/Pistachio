#include "EditorLayer.h"
#include "imgui.h"

#include <glm/gtc/type_ptr.hpp>

namespace Pistachio {

	EditorLayer::EditorLayer()
		:Layer("EditorLayer"), m_CameraController(1280, 720) {

	
	}

	void EditorLayer::OnAttach() {
		m_CameraController.SetZoomLevel(4.0f);

		m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");
		m_EmojiTexture = Texture2D::Create("assets/textures/emoji.png");
		m_SpriteSheet = Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png", {128, 128} /*stride*/);

		m_TextureMap['W'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, {11, 11});
		m_TextureMap['D'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, {6, 11});
		m_TextureMap['G'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, {1, 11});

		// framebuffer
		FrameBufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = FrameBuffer::Create(fbSpec);

		// Scene
		m_ActiveScene = CreateRef<Scene>();
		auto square = m_ActiveScene->CreateEntity();
		m_ActiveScene->Reg().emplace<TransformComponent>(square);
		m_ActiveScene->Reg().emplace<SpriteComponent>(square, glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});
		m_SquareEntity = square;

		//square.

	}

	void EditorLayer::OnDetach() {
	}

	void EditorLayer::OnUpdate(Timestep ts) {
		PTC_PROFILE_FUNCTION();

		// Update
		if (m_ViewportFocused) {
			m_CameraController.OnUpdate(ts);
		}

		// Renderer Prep
		{
			PTC_PROFILE_SCOPE("Renderer Prep");
			m_Framebuffer->Bind();
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();
		}
		
	
		// sprite sheet test
		{
			Renderer2D::BeginScene(m_CameraController.GetCamera());
			//Renderer2D::DrawQuad(m_Texture, { -50.0f, -50.0f, -0.5f }, glm::radians(0.0f), { 100.0f, 100.0f }, 50.0f);
			//Renderer2D::DrawQuad(m_TextureMap['W'], { 0.0f, 0.0f, 1.0f }, glm::radians(0.0f), { 1.0f, 1.0f });
			//Renderer2D::DrawQuad(m_TextureMap['D'], { 0.0f, -1.0f, 1.0f }, glm::radians(0.0f), { 1.0f, 1.0f });
			//Renderer2D::DrawQuad(m_TextureMap['G'], { -1.0f, -1.0f, 1.0f }, glm::radians(0.0f), { 1.0f, 1.0f });
			//Renderer2D::DrawQuad({1.0f, 1.0f, 0.0f, 1.0f}, { -1.0f, 2.0f, 1.0f }, glm::radians(30.0f), { 1.0f, 1.0f });
			// Draw scene
			m_ActiveScene->OnUpdate(ts);


			Renderer2D::EndScene();
			m_Framebuffer->Unbind();
		}


	}

	void EditorLayer::OnImGuiRender() {
		PTC_PROFILE_FUNCTION();


		static bool DockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &DockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		{
			ImGui::Begin("Settings");

			auto& squarecolor = m_ActiveScene->Reg().get<SpriteComponent>(m_SquareEntity).Color;
			ImGui::ColorEdit4("SquareColor", glm::value_ptr(squarecolor));
				
			ImGui::End();
		}

		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
			ImGui::Begin("Viewport");
			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			//PTC_CORE_TRACE("viewport size: {0}, {1}", viewportPanelSize.x, viewportPanelSize.y);
			if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize) 
				&& viewportPanelSize.x > 0 && viewportPanelSize.y > 0) {
				m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
				m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

				m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			}
			uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)textureID, viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			ImGui::End();
			ImGui::PopStyleVar();
		}

		ImGui::End();
			
	}

	void EditorLayer::OnEvent(Event& event) {
		if (m_ViewportHovered) {
			m_CameraController.OnEvent(event);
		}
	}
}

