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
		Ref<VertexArray> CircleVertexArray;
		ShaderLibrary TheShaderLibrary;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init() {
		PTC_PROFILE_FUNCTION();

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

		/// Circle primitive (default 8 sides)
		s_Data->CircleVertexArray = VertexArray::Create();
		// vertex buffer
		float CircleVertices[3 * 9] = {
			0.0f,	0.0f,	0.0f,
			1.0f,	0.0f,	0.0f,
			0.707f,	0.707f,	0.0f,
			0.0f,	1.0f,	0.0f,
			-0.707f,0.707f,	0.0f,
			-1.0f,	0.0f,	0.0f,
			-0.707f,-0.707f,0.0f,
			0.0f,	-1.0f,	0.0f,
			0.707f, -0.707f,0.0f
		};
		auto CircleVB = VertexBuffer::Create(CircleVertices, sizeof(CircleVertices));
		// vertex buffer layout
		BufferLayout CircleLayout = {
			{ ShaderDataType::Float3, "a_Position" }
		};
		CircleVB->SetLayout(CircleLayout);
		s_Data->CircleVertexArray->AddVertexBuffer(CircleVB);
		// index buffer
		unsigned int CircleIndices[24] = { 
			0, 1, 2, 
			0, 2, 3, 
			0, 3, 4, 
			0, 4, 5,
			0, 5, 6,
			0, 6, 7,
			0, 7, 8,
			0, 8, 1};
		auto CircleIB = IndexBuffer::Create(CircleIndices, sizeof(CircleIndices) / sizeof(uint32_t));
		s_Data->CircleVertexArray->SetIndexBuffer(CircleIB);

		// flat color shader
		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		// texture shader
		auto textureShader = s_Data->TheShaderLibrary.Load("assets/shaders/Texture.glsl");
		textureShader->Bind();
		textureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown() {
		PTC_PROFILE_FUNCTION();

		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		PTC_PROFILE_FUNCTION();

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
		PTC_PROFILE_FUNCTION();

		s_Data->TheShaderLibrary.Get("Texture")->Bind();
		s_Data->TheShaderLibrary.Get("Texture")
			->SetFloat4("u_Color", color);
		s_Data->WhiteTexture->Bind();

		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
		s_Data->TheShaderLibrary.Get("Texture")
			->SetMat4("u_Transform", translate * rotate * scale);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const Ref<Texture2D>& tex, const glm::vec2& position, const glm::vec2& size, const float rotation) {
		DrawQuad(tex, { position.x, position.y, 0.0f }, size, rotation);
	}

	void Renderer2D::DrawQuad(const Ref<Texture2D>& tex, const glm::vec3& position, const glm::vec2& size, const float rotation) {
		PTC_PROFILE_FUNCTION();

		s_Data->TheShaderLibrary.Get("Texture")->SetFloat4("u_Color", glm::vec4(1.0f));
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

	void Renderer2D::DrawCircle(const glm::vec4& color, const glm::vec2& position, const float radius, const float rotation, const uint32_t sides) {
		DrawCircle(color, { position.x, position.y, 0.0f }, radius, rotation, sides);
	}

	void Renderer2D::DrawCircle(const glm::vec4& color, const glm::vec3& position, const float radius, const float rotation, const uint32_t sides) {
		PTC_PROFILE_FUNCTION();

		s_Data->TheShaderLibrary.Get("Texture")->Bind();
		s_Data->TheShaderLibrary.Get("Texture")
			->SetFloat4("u_Color", color);
		s_Data->WhiteTexture->Bind();

		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), { radius, radius, 1.0f });
		s_Data->TheShaderLibrary.Get("Texture")
			->SetMat4("u_Transform", translate * rotate * scale);

		s_Data->CircleVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CircleVertexArray);


	}

}