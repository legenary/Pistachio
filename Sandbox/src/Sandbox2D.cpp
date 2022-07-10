#include "Sandbox2D.h"
#include "imgui.h"

#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_CameraController(1280, 720) {

	
}

void Sandbox2D::OnAttach() {
	m_Texture = Pistachio::Texture2D::Create("assets/textures/Checkerboard.png");
	m_EmojiTexture = Pistachio::Texture2D::Create("assets/textures/emoji.png");

	m_World = new Pistachio::World();
	m_World->Init();

	//m_World->addBoxComponent(glm::vec2(0.0f, 0.0f), -90, glm::vec2(5.0f, 2.0f));
	//m_World->addBoxComponent(glm::vec2(2.0f, 4.0f), 10, glm::vec2(4.0f, 1.0f));
	m_World->addBoxComponent(glm::vec2(0.0f, -9.0f), 10, glm::vec2(20.0f, 5.0f), false);
	m_World->addBoxComponent(glm::vec2(0.0f, -9.0f), -10, glm::vec2(20.0f, 5.0f), false);

	for (float x = -4.5f; x < 4.5f; x += 2.0f) {
		for (float y = 0.0f; y <10.0f; y += 2.0f) {
			m_World->addCircleComponent(glm::vec2(x, y), 0.8f);
		}
	}
}

void Sandbox2D::OnDetach() {
}

void Sandbox2D::OnUpdate(Pistachio::Timestep ts) {
	PTC_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(ts);

	// step simulate world
	{
		PTC_PROFILE_SCOPE("Physics step simulation");
		m_World->getPhysicsWorld()->Step(ts, 6, 2);
	}

	// Renderer Prep
	{
		PTC_PROFILE_SCOPE("Renderer Prep");
		Pistachio::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Pistachio::RenderCommand::Clear();
	}

	// draw calls
	{
		PTC_PROFILE_SCOPE("Renderer Draw");
		Pistachio::Renderer2D::BeginScene(m_CameraController.GetCamera());

		int bodyCount = m_World->getPhysicsWorld()->GetBodyCount();
		for (int i = 0; i < bodyCount; i++) {
			m_World->getComponentByIndex(i)->Draw({ 0.2f, 0.8f, 0.3f, 1.0f });
		}
	
		Pistachio::Renderer2D::DrawQuad({ 0.8f, 0.3f, 0.2f, 1.0f }, { 0.0f, 0.0f }, { 4.5f, 1.5f }, 10);
		Pistachio::Renderer2D::DrawQuad(m_Texture, { 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f });
	
		Pistachio::Renderer2D::EndScene();
	}

	
}

void Sandbox2D::OnImGuiRender() {
	PTC_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit3("SquareColor", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Pistachio::Event& event) {
	m_CameraController.OnEvent(event);
}
