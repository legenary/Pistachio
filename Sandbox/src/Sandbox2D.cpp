#include "Sandbox2D.h"
#include "imgui.h"

#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_CameraController(1280, 720) {

	
}

void Sandbox2D::OnAttach() {
	m_CameraController.SetZoomLevel(4.0f);

	m_Texture = Pistachio::Texture2D::Create("assets/textures/Checkerboard.png");
	m_EmojiTexture = Pistachio::Texture2D::Create("assets/textures/emoji.png");
	m_SpriteSheet = Pistachio::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png", {128, 128} /*stride*/);

	m_TextureMap['W'] = Pistachio::SubTexture2D::CreateFromCoords(m_SpriteSheet, {11, 11});
	m_TextureMap['D'] = Pistachio::SubTexture2D::CreateFromCoords(m_SpriteSheet, {6, 11});
	m_TextureMap['G'] = Pistachio::SubTexture2D::CreateFromCoords(m_SpriteSheet, {1, 11});
	
	// Particle System Init here
	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 1.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };

	// framebuffer
	Pistachio::FrameBufferSpecification fbSpec;
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_Framebuffer = Pistachio::FrameBuffer::Create(fbSpec);

	//m_World = new Pistachio::World();
	//m_World->Init();

	//m_World->addBoxComponent(glm::vec2(0.0f, 0.0f), -90, glm::vec2(5.0f, 2.0f));
	//m_World->addBoxComponent(glm::vec2(2.0f, 4.0f), 10, glm::vec2(4.0f, 1.0f));
	//m_World->addBoxComponent(glm::vec2(4.0f, -7.0f), 10, glm::vec2(10.0f, 1.0f), false);
	//m_World->addBoxComponent(glm::vec2(-4.0f, -7.0f), -10, glm::vec2(10.0f, 1.0f), false);

	//for (float x = -4.5f; x < 4.5f; x += 2.0f) {
	//	for (float y = 0.0f; y <10.0f; y += 2.0f) {
	//		m_World->addCircleComponent(glm::vec2(x, y), 0.8f);
	//	}
	//}
}

void Sandbox2D::OnDetach() {
}

void Sandbox2D::OnUpdate(Pistachio::Timestep ts) {
	PTC_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(ts);

	//// step simulate world
	//{
	//	PTC_PROFILE_SCOPE("Physics step simulation");
	//	m_World->getPhysicsWorld()->Step(ts, 6, 2);
	//}

	// Renderer Prep
	{
		PTC_PROFILE_SCOPE("Renderer Prep");
		m_Framebuffer->Bind();

		Pistachio::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Pistachio::RenderCommand::Clear();
	}

	//// draw calls
	//{
	//	PTC_PROFILE_SCOPE("Renderer Draw");
	//	Pistachio::Renderer2D::BeginScene(m_CameraController.GetCamera());

	//	//int bodyCount = m_World->getPhysicsWorld()->GetBodyCount();
	//	//for (int i = 0; i < bodyCount; i++) {
	//	//	//m_World->getComponentByIndex(i)->Draw({ 0.2f, 0.8f, 0.3f, 1.0f });
	//	//}
	//
	//	Pistachio::Renderer2D::DrawQuad({ 0.3f, 0.8f, 0.2f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(10.0f));
	//	Pistachio::Renderer2D::DrawQuad({ 0.8f, 0.3f, 0.2f, 1.0f }, { 0.0f, 0.0f }, { 4.5f, 1.5f }, glm::radians(-10.0f));
	//	Pistachio::Renderer2D::DrawQuad(m_Texture, { -50.0f, -50.0f, -0.1f }, { 100.0f, 100.0f }, glm::radians(0.0f), 10.0f);
	//	
	//	Pistachio::Renderer2D::EndScene();
	//}

	// Particle system
	{
		if (Pistachio::Input::IsMouseButtonPressed(PTC_MOUSE_BUTTON_LEFT))
		{
			auto [x, y] = Pistachio::Input::GetMousePosition();
			auto width = Pistachio::Application::Get().GetWindow().GetWidth();
			auto height = Pistachio::Application::Get().GetWindow().GetHeight();

			auto bounds = m_CameraController.GetBounds();
			auto pos = m_CameraController.GetCamera().GetPosition();
			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
			m_Particle.Position = { x + pos.x, y + pos.y };
			for (int i = 0; i < 5; i++)
				m_ParticleSystem.Emit(m_Particle);
		}
		m_ParticleSystem.OnUpdate(ts);
		m_ParticleSystem.OnRender(m_CameraController.GetCamera());
	}
	
	
	// sprite sheet test
	{
		Pistachio::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Pistachio::Renderer2D::DrawQuad(m_Texture, { -50.0f, -50.0f, 0.2f }, { 100.0f, 100.0f }, glm::radians(0.0f), 50.0f);
		//Pistachio::Renderer2D::DrawQuad(m_SpriteSheet, { 0.0f, 0.0f, 1.0f }, { 3.0f, 3.0f });
		Pistachio::Renderer2D::DrawQuad(m_TextureMap['W'], {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f});
		Pistachio::Renderer2D::DrawQuad(m_TextureMap['D'], {0.0f, -1.0f, 1.0f}, {1.0f, 1.0f});
		Pistachio::Renderer2D::DrawQuad(m_TextureMap['G'], {-1.0f, -1.0f, 1.0f}, {1.0f, 1.0f});
		Pistachio::Renderer2D::EndScene();
		m_Framebuffer->Unbind();
	}

	

}

void Sandbox2D::OnImGuiRender() {
	PTC_PROFILE_FUNCTION();

	static bool dockingEnabled = true;
	if (dockingEnabled) {
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
				if (ImGui::MenuItem("Exit")) Pistachio::Application::Get().Close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");
		ImGui::ColorEdit3("SquareColor", glm::value_ptr(m_SquareColor));
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ 1280.0f , 720.0f });
		ImGui::End();


		ImGui::End();

	}
	else {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("SquareColor", glm::value_ptr(m_SquareColor));
		uint32_t textureID = m_Texture->GetRendererID();
		ImGui::Image((void*)textureID, ImVec2{ 64.0f ,64.0f });
		ImGui::End();
	}
}

void Sandbox2D::OnEvent(Pistachio::Event& event) {
	m_CameraController.OnEvent(event);
}
