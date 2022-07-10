#pragma once

#include "RenderCommand.h"
#include "Renderer2D.h"

#include <Pistachio/Renderer/OrthographicCamera.h>
#include <Pistachio/Renderer/Shader.h>


namespace Pistachio {

	class Renderer {
	public:
		static void Init() {
			PTC_PROFILE_FUNCTION();

			RenderCommand::Init();
			Renderer2D::Init();
		}
		static void OnWindoeResize(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		//static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData{
			glm::mat4 ViewProjectionMatrix;
		};
		static SceneData* m_SceneData;
	};

}