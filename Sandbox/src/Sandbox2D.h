#pragma once

#include "Pistachio.h"

class Sandbox2D : public Pistachio::Layer {
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Pistachio::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Pistachio::Event& event) override;

private:
	Pistachio::ShaderLibrary m_ShaderLibrary;
	// first triangle
	Pistachio::Ref<Pistachio::VertexArray> m_TriVA;

	// second square
	Pistachio::Ref<Pistachio::VertexArray> m_SquareVA;

	// third texture
	Pistachio::Ref<Pistachio::Texture2D> m_Texture, m_EmojiTexture;

	Pistachio::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

