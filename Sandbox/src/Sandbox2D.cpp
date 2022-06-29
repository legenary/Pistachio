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
}

void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnUpdate(Pistachio::Timestep ts) {
	//PTC_TRACE("Delta time: {0}s", ts.GetSeconds());

	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Pistachio::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Pistachio::RenderCommand::Clear();

	// draw calls
	Pistachio::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Pistachio::Renderer2D::DrawQuad({ 0.8f, 0.2f, 0.3f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 10);
	Pistachio::Renderer2D::DrawQuad({ 0.2f, 0.8f, 0.3f, 1.0f }, { 1.0f, 0.5f }, { 0.5f, 0.5f }, -10);
	Pistachio::Renderer2D::DrawQuad(m_Texture, { 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f });
	
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
