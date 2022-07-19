#include "ptcpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RenderCommand.h"


namespace Pistachio {

	struct QuadVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
		// TODO: maskid
	};
	struct CircleVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		// TODO: texcoord, texid, maskid
	};


	struct Renderer2DData {
		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxQuadVertices = MaxQuads * 4;
		const uint32_t MaxQuadIndices = MaxQuads * 6;
		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		
		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		const uint32_t MaxCircles = 10000;
		const uint32_t MaxCircleVertices = MaxCircles * 9;
		const uint32_t MaxCircleIndices = MaxCircles * 24;
		uint32_t CircleIndexCount = 0;
		QuadVertex* CircleVertexBufferBase = nullptr;
		QuadVertex* CircleVertexBufferPtr = nullptr;


		static const uint32_t MaxTextureSlots = 32;	// TODO: RenderCaps
		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		Ref<Texture2D> WhiteTexture;
		uint32_t TextureSlotIndex = 1; // slot 0 being white texture

		ShaderLibrary TheShaderLibrary;
	};

	static Renderer2DData s_Data;
	

	void Renderer2D::Init() {
		PTC_PROFILE_FUNCTION();

		/// Quad primitive
		s_Data.QuadVertexArray = VertexArray::Create();
		//// vertex buffer
		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxQuadVertices * sizeof(QuadVertex));
		// vertex buffer layout
		BufferLayout QuadLayout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color"},
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex"},
			{ ShaderDataType::Float, "a_TilingFactor"}
		};
		s_Data.QuadVertexBuffer->SetLayout(QuadLayout);
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxQuadVertices];

		// index buffer
		uint32_t* quadIndices = new uint32_t[s_Data.MaxQuadIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxQuadIndices; i += 6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;
			offset += 4;
		}
		auto QuadIB = IndexBuffer::Create(quadIndices, s_Data.MaxQuadIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(QuadIB);
		delete[] quadIndices;


		
		///// Circle primitive (default 8 sides)
		//s_Data.CircleVertexArray = VertexArray::Create();
		//// vertex buffer
		//s_Data.CircleVertexBuffer = VertexBuffer::Create(s_Data.MaxCircleVertices * sizeof(CircleVertex));
		//// vertex buffer layout
		//BufferLayout CircleLayout = {
		//	{ ShaderDataType::Float3, "a_Position" },
		//	{ ShaderDataType::Float4, "a_Color"}
		//};
		//s_Data.CircleVertexBuffer->SetLayout(CircleLayout);
		//s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
		//// index buffer
		//uint32_t* circleIndices = new uint32_t[s_Data.MaxCircleIndices];
		//offset = 0;
		//for (uint32_t i = 0; i < s_Data.MaxQuadIndices; i += 24) {
		//	circleIndices[i + 0] = offset + 0;
		//	circleIndices[i + 1] = offset + 1;
		//	circleIndices[i + 2] = offset + 2;
		//	circleIndices[i + 3] = offset + 0;
		//	circleIndices[i + 4] = offset + 2;
		//	circleIndices[i + 5] = offset + 3;
		//	circleIndices[i + 6] = offset + 0;
		//	circleIndices[i + 7] = offset + 3;
		//	circleIndices[i + 8] = offset + 4;
		//	circleIndices[i + 9] = offset + 0;
		//	circleIndices[i + 10] = offset + 4;
		//	circleIndices[i + 11] = offset + 5;
		//	circleIndices[i + 12] = offset + 0;
		//	circleIndices[i + 13] = offset + 5;
		//	circleIndices[i + 14] = offset + 6;
		//	circleIndices[i + 15] = offset + 0;
		//	circleIndices[i + 16] = offset + 6;
		//	circleIndices[i + 17] = offset + 7;
		//	circleIndices[i + 18] = offset + 0;
		//	circleIndices[i + 19] = offset + 7;
		//	circleIndices[i + 20] = offset + 8;
		//	circleIndices[i + 21] = offset + 0;
		//	circleIndices[i + 22] = offset + 8;
		//	circleIndices[i + 23] = offset + 1;
		//	offset += 9;
		//}
		//auto CircleIB = IndexBuffer::Create(circleIndices, s_Data.MaxCircleIndices);
		//s_Data.CircleVertexArray->SetIndexBuffer(CircleIB);
		//delete[] circleIndices;
		

		// flat color shader
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		// texture shader
		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++) {
			samplers[i] = i;
		}


		auto textureShader = s_Data.TheShaderLibrary.Load("assets/shaders/Texture.glsl");
		textureShader->Bind();
		textureShader->SetIntArray("u_Texture", samplers, s_Data.MaxTextureSlots);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;
	}

	void Renderer2D::Shutdown() {
		PTC_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		PTC_PROFILE_FUNCTION();

		s_Data.TheShaderLibrary.Get("Texture")->Bind();
		s_Data.TheShaderLibrary.Get("Texture")
			->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.QuadIndexCount = 0;
		//s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;
		//s_Data.CircleIndexCount = 0;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene() {
		PTC_PROFILE_FUNCTION();
		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
		//dataSize = (uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase;
		//s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);
		Flush();
	}

	void Renderer2D::Flush() {
		// Bind texture
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) {
			s_Data.TextureSlots[i]->Bind(i);
		}

		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		//RenderCommand::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec4& color, const glm::vec2& position, const glm::vec2& size, const float rotation) {
		DrawQuad(color, { position.x, position.y, 0.0f }, size, rotation);
	}

	void Renderer2D::DrawQuad(const glm::vec4& color, const glm::vec3& position, const glm::vec2& size, const float rotation) {
		PTC_PROFILE_FUNCTION();

		const float texIndex = 0.0f;	// whiteTexture
		const float tilingFactor = 1.0f;

		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f};
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = {position.x + size.x, position.y, 0.0f};
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f};
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f};
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f};
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		// drawing is done at the "flush" step
	}

	void Renderer2D::DrawQuad(const Ref<Texture2D>& tex, const glm::vec2& position, const glm::vec2& size, const float rotation, const float tilingFactor) {
		DrawQuad(tex, { position.x, position.y, 0.0f }, size, rotation, tilingFactor);
	}

	void Renderer2D::DrawQuad(const Ref<Texture2D>& tex, const glm::vec3& position, const glm::vec2& size, const float rotation, const float tilingFactor) {
		PTC_PROFILE_FUNCTION();

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *tex.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = tex;
			s_Data.TextureSlotIndex++;
		} 

		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
		/*
		s_Data.TheShaderLibrary.Get("Texture")->SetFloat4("u_Color", glm::vec4(1.0f));
		s_Data.TheShaderLibrary.Get("Texture")->SetFloat("u_TilingFactor", tilingFactor);
		tex->Bind();

		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1));
		s_Data.TheShaderLibrary.Get("Texture")->Bind();
		s_Data.TheShaderLibrary.Get("Texture")
			->SetMat4("u_Transform", translate * rotate * scale);

		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
		*/
	}


	void Renderer2D::DrawCircle(const glm::vec4& color, const glm::vec2& position, const float radius, const float rotation, const uint32_t sides) {
		DrawCircle(color, { position.x, position.y, 0.0f }, radius, rotation, sides);
	}

	void Renderer2D::DrawCircle(const glm::vec4& color, const glm::vec3& position, const float radius, const float rotation, const uint32_t sides) {
		PTC_PROFILE_FUNCTION();

		s_Data.CircleVertexBufferPtr->Position = position;
		s_Data.CircleVertexBufferPtr->Color = color;
		s_Data.CircleVertexBufferPtr++;
			   
		s_Data.CircleVertexBufferPtr->Position = { position.x + radius, position.y, 0.0f };
		s_Data.CircleVertexBufferPtr->Color = color;
		s_Data.CircleVertexBufferPtr++;
			   
		s_Data.CircleVertexBufferPtr->Position = { position.x + radius * 0.707, position.y + radius * 0.707, 0.0f };
		s_Data.CircleVertexBufferPtr->Color = color;
		s_Data.CircleVertexBufferPtr++;
			   
		s_Data.CircleVertexBufferPtr->Position = { position.x, position.y + radius, 0.0f };
		s_Data.CircleVertexBufferPtr->Color = color;
		s_Data.CircleVertexBufferPtr++;

		s_Data.CircleVertexBufferPtr->Position = { position.x - radius * 0.707, position.y + radius * 0.707, 0.0f };
		s_Data.CircleVertexBufferPtr->Color = color;
		s_Data.CircleVertexBufferPtr++;

		s_Data.CircleVertexBufferPtr->Position = { position.x - radius, position.y, 0.0f };
		s_Data.CircleVertexBufferPtr->Color = color;
		s_Data.CircleVertexBufferPtr++;

		s_Data.CircleVertexBufferPtr->Position = { position.x - radius * 0.707, position.y - radius * 0.707, 0.0f };
		s_Data.CircleVertexBufferPtr->Color = color;
		s_Data.CircleVertexBufferPtr++;

		s_Data.CircleVertexBufferPtr->Position = { position.x, position.y - radius, 0.0f };
		s_Data.CircleVertexBufferPtr->Color = color;
		s_Data.CircleVertexBufferPtr++;

		s_Data.CircleVertexBufferPtr->Position = { position.x + radius * 0.707, position.y - radius * 0.707, 0.0f };
		s_Data.CircleVertexBufferPtr->Color = color;
		s_Data.CircleVertexBufferPtr++;

		s_Data.CircleIndexCount += 24;

		/*s_Data.TheShaderLibrary.Get("Texture")->Bind();
		s_Data.TheShaderLibrary.Get("Texture")
			->SetFloat4("u_Color", color);
		s_Data.TheShaderLibrary.Get("Texture")->SetFloat("u_TilingFactor", 1.0f);
		s_Data.WhiteTexture->Bind();

		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), { radius, radius, 1.0f });
		s_Data.TheShaderLibrary.Get("Texture")
			->SetMat4("u_Transform", translate * rotate * scale);

		s_Data.CircleVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.CircleVertexArray);*/


	}

}