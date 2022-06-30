#include "Sandbox2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui.h"

#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D")
	, m_CameraController(1280, 720) {
	
}

void Sandbox2D::OnAttach() {
	m_Texture = Pistachio::Texture2D::Create("assets/textures/Checkerboard.png");
	m_EmojiTexture = Pistachio::Texture2D::Create("assets/textures/emoji.png");

	m_Physics = new Pistachio::Physics();
	m_Physics->Init();

	m_Physics->addBox(glm::vec2(0.0f, -90.0f), 0, glm::vec2(100.0f, 50.0f), false);
	m_Physics->addBox(glm::vec2(0.0f, 0.0f), 0, glm::vec2(30.0f, 30.0f));
	m_Physics->addBox(glm::vec2(25.0f, 40.0f), 10, glm::vec2(40.0f, 10.0f));

}

void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnUpdate(Pistachio::Timestep ts) {
	//PTC_TRACE("Delta time: {0}s", ts.GetSeconds());
	Pistachio::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Pistachio::RenderCommand::Clear();

	// step simulate world
	m_Physics->getWorld()->Step(ts.GetSeconds(), 6, 2);
	//m_World->Step(ts, 6, 2);

	// Update
	m_CameraController.OnUpdate(ts);

	// draw calls
	Pistachio::Renderer2D::BeginScene(m_CameraController.GetCamera());

	for (int i = 0; i < 3; i++) {
		m_Physics->getObjectByIndex(i).Draw({ 0.2f, 0.8f, 0.3f, 1.0f });
	}

	
	//Pistachio::Renderer2D::DrawQuad({ 0.8f, 0.3f, 0.2f, 1.0f }, { 0.0f, 0.0f }, { 40.5f, 10.5f }, 10);
	//Pistachio::Renderer2D::DrawQuad(m_Texture, { 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f });
	
	Pistachio::Renderer2D::EndScene();

	//Renderer::Flush();
	
}

void Sandbox2D::OnImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("SquareColor", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Pistachio::Event& event) {
	m_CameraController.OnEvent(event);
}
