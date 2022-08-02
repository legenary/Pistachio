#pragma once

#include "Pistachio/Renderer/OrthographicCamera.h"
#include "Pistachio/Renderer/Camera.h"
#include "Pistachio/Renderer/Texture.h"
#include "Pistachio/Renderer/SubTexture2D.h"

#include "Pistachio/Renderer/EditorCamera.h"

namespace Pistachio {

	class Renderer2D {
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush(); 

		// primitives 
		static void DrawQuad(const glm::vec4& color, const glm::vec2& position = {0.0f, 0.0f}, const float rotation = 0, const glm::vec2& size = { 1.0f, 1.0f });
		static void DrawQuad(const glm::vec4& color, const glm::vec3& position = { 0.0f, 0.0f, 0.0f }, const float rotation = 0, const glm::vec2& size = { 1.0f, 1.0f });
		static void DrawQuad(const Ref<Texture2D>& tex, const glm::vec2& position = { 0.0f, 0.0f }, const float rotation = 0, const glm::vec2& size = { 1.0f, 1.0f }, const float tilingFactor = 1.0f);
		static void DrawQuad(const Ref<Texture2D>& tex, const glm::vec3& position = { 0.0f, 0.0f, 0.0f }, const float rotation = 0, const glm::vec2& size = { 1.0f, 1.0f }, const float tilingFactor = 1.0f);
		static void DrawQuad(const Ref<SubTexture2D>& subtex, const glm::vec2& position = { 0.0f, 0.0f }, const float rotation = 0, const glm::vec2& size = { 1.0f, 1.0f }, const float tilingFactor = 1.0f);
		static void DrawQuad(const Ref<SubTexture2D>& subtex, const glm::vec3& position = { 0.0f, 0.0f, 0.0f }, const float rotation = 0, const glm::vec2& size = { 1.0f, 1.0f }, const float tilingFactor = 1.0f);
	
		static void DrawQuad(const glm::vec4& color, const glm::mat4& transform);
		static void DrawQuad(const Ref<Texture2D>& tex, const glm::mat4& transform, const float tilingFactor = 1.0f);
		static void DrawQuad(const Ref<SubTexture2D>& subtex, const glm::mat4& transform, const float tilingFactor = 1.0f);

	};

}