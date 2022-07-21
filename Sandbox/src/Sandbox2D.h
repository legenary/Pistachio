#pragma once

#include "Pistachio.h"

#include "ParticleSystem.h"

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
	//Pistachio::World* m_World;
	
	Pistachio::Ref<Pistachio::Texture2D> m_Texture, m_EmojiTexture, m_SpriteSheet;
	Pistachio::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };

	ParticleProps m_Particle;
	ParticleSystem m_ParticleSystem;

	std::unordered_map<char, Pistachio::Ref<Pistachio::SubTexture2D>> m_TextureMap;
};

