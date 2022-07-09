#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"

namespace Pistachio {

	class Renderer2D {
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// primitives 
		static void DrawQuad(const glm::vec4& color, const glm::vec2& position = {0.0f, 0.0f}, const glm::vec2& size = { 1.0f, 1.0f }, const float rotation = 0);
		static void DrawQuad(const glm::vec4& color, const glm::vec3& position = { 0.0f, 0.0f, 0.0f }, const glm::vec2& size = { 1.0f, 1.0f }, const float rotation = 0);
		static void DrawQuad(const Ref<Texture2D>& tex, const glm::vec2& position = { 0.0f, 0.0f }, const glm::vec2& size = { 1.0f, 1.0f }, const float rotation = 0);
		static void DrawQuad(const Ref<Texture2D>& tex, const glm::vec3& position = { 0.0f, 0.0f, 0.0f }, const glm::vec2& size = { 1.0f, 1.0f }, const float rotation = 0);
	
		static void DrawCircle(const glm::vec4& color, const glm::vec2& position = { 0.0f, 0.0f }, const float radius = 1.0f, const float rotation = 0, const uint32_t sides = 8);
		static void DrawCircle(const glm::vec4& color, const glm::vec3& position = { 0.0f, 0.0f, 0.0f }, const float radius = 1.0f, const float rotation = 0, const uint32_t sides = 8);
	};

}