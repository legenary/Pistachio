#include "ptcpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RenderCommand.h"


namespace Pistachio {

	struct Renderer2DStorage {
		Ref<VertexArray> QuadVertexArray;
		ShaderLibrary TheShaderLibrary;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init() {
		s_Data = new Renderer2DStorage();

		/// Quad primitive
		s_Data->QuadVertexArray = VertexArray::Create();
		// vertex buffer
		float QuadVertices[4 * 5] = {
			-0.5f,	-0.5f,	0.0f,	0.0f, 0.0f,
			0.5f,	-0.5f,	0.0f,	1.0f, 0.0f,
			0.5f,	0.5f,	0.0f,	1.0f, 1.0f,
			-0.5f,	0.5f,	0.0f,	0.0f, 1.0f
		};
		auto QuadVB = VertexBuffer::Create(QuadVertices, sizeof(QuadVertices));
		// vertex buffer layout
		BufferLayout QuadLayout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		};
		QuadVB->SetLayout(QuadLayout);
		s_Data->QuadVertexArray->AddVertexBuffer(QuadVB);
		// index buffer
		unsigned int QuadIndices[6] = { 0, 1, 2, 2, 3, 0 };
		auto QuadIB = IndexBuffer::Create(QuadIndices, sizeof(QuadIndices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(QuadIB);
		// shader
		s_Data->TheShaderLibrary.Load("assets/shaders/FlatColor.glsl");
		auto textureShader = s_Data->TheShaderLibrary.Load("assets/shaders/Texture.glsl");
		textureShader->Bind();
		textureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown() {
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		s_Data->TheShaderLibrary.Get("FlatColor")->Bind();
		s_Data->TheShaderLibrary.Get("FlatColor")
			->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		s_Data->TheShaderLibrary.Get("Texture")->Bind();
		s_Data->TheShaderLibrary.Get("Texture")
			->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene() {
	}

	void Renderer2D::DrawQuad(const glm::vec4& color, const glm::vec2& position, const glm::vec2& size, const float rotation) {
		DrawQuad(color, { position.x, position.y, 0.0f }, size, rotation);
	}

	void Renderer2D::DrawQuad(const glm::vec4& color, const glm::vec3& position, const glm::vec2& size, const float rotation) {
		s_Data->TheShaderLibrary.Get("FlatColor")->Bind();
		s_Data->TheShaderLibrary.Get("FlatColor")
			->SetFloat4("u_Color", color);

		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1));
		s_Data->TheShaderLibrary.Get("FlatColor")
			->SetMat4("u_Transform", translate * rotate * scale);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const Ref<Texture2D>& tex, const glm::vec2& position, const glm::vec2& size, const float rotation) {
		DrawQuad(tex, { position.x, position.y, 0.0f }, size, rotation);
	}

	void Renderer2D::DrawQuad(const Ref<Texture2D>& tex, const glm::vec3& position, const glm::vec2& size, const float rotation) {
		tex->Bind();

		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1));
		s_Data->TheShaderLibrary.Get("Texture")->Bind();
		s_Data->TheShaderLibrary.Get("Texture")
			->SetMat4("u_Transform", translate * rotate * scale);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

}