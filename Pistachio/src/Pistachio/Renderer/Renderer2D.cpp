#include "ptcpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "Pistachio/Core/Utility.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RenderCommand.h"

#include "Pistachio/Scene/Component.h"


namespace Pistachio {

	struct QuadVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
		// TODO: maskid

		int EntityID;
	};

	struct CircleVertex {
		glm::vec3 Position;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;
		// TODO: maskid

		int EntityID;
	};

	struct Renderer2DData {
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxQuadVertices = MaxQuads * 4;
		static const uint32_t MaxQuadIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;
		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;
		glm::vec4 QuadVertexPositions[4] = {
			{-0.5f, -0.5f, 0.0f, 1.0f},
			{0.5f, -0.5f, 0.0f, 1.0f},
			{0.5f, 0.5f, 0.0f, 1.0f},
			{-0.5f, 0.5f, 0.0f, 1.0f}
		};
		int QuadCount = 0;

		static const uint32_t MaxCircles = 10000;
		static const uint32_t MaxCircleVertices = MaxCircles * 4;
		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;
		glm::vec4 CircleVertexPositions[4] = {
			{-0.5f, -0.5f, 0.0f, 1.0f},
			{0.5f, -0.5f, 0.0f, 1.0f},
			{0.5f, 0.5f, 0.0f, 1.0f},
			{-0.5f, 0.5f, 0.0f, 1.0f}
		};
		int CircleCount = 0;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		Ref<Texture2D> WhiteTexture;
		uint32_t TextureSlotIndex = 1;	// slot start from 1
										// slot 0 being white texture

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
			{ ShaderDataType::Float, "a_TilingFactor"},
			{ ShaderDataType::Int, "a_EntityID"}
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

		/// Circle primitive
		s_Data.CircleVertexArray = VertexArray::Create();
		//// vertex buffer
		s_Data.CircleVertexBuffer = VertexBuffer::Create(s_Data.MaxCircleVertices * sizeof(CircleVertex));
		// vertex buffer layout
		BufferLayout CircleLayout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_LocalPosition" },
			{ ShaderDataType::Float4, "a_Color"},
			{ ShaderDataType::Float, "a_Thickness"},
			{ ShaderDataType::Float, "a_Fade"},
			{ ShaderDataType::Int, "a_EntityID"}
		};
		s_Data.CircleVertexBuffer->SetLayout(CircleLayout);
		s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
		s_Data.CircleVertexArray->SetIndexBuffer(QuadIB);	// Use quad IB
		s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxCircleVertices];

		// flat color shader
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		// texture shader
		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++) {
			samplers[i] = i;
		}

		//auto quadShader = s_Data.TheShaderLibrary.Load("assets/shaders/QuadShader.glsl");
		//quadShader->Bind();
		//quadShader->SetIntArray("u_Texture", samplers, s_Data.MaxTextureSlots);
		s_Data.TheShaderLibrary.Load("assets/shaders/QuadShader.glsl");
		s_Data.TheShaderLibrary.Load("assets/shaders/CircleShader.glsl");

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;
	}

	void Renderer2D::Shutdown() {
		PTC_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform) {
		PTC_PROFILE_FUNCTION();
		glm::mat4 viewProj = camera.GetProjectionMatrix() * glm::inverse(transform);
		
		s_Data.TheShaderLibrary.Get("QuadShader")->Bind();
		s_Data.TheShaderLibrary.Get("QuadShader")
			->SetMat4("u_ViewProjection", viewProj);
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.QuadIndexCount = 0;

		s_Data.TheShaderLibrary.Get("CircleShader")->Bind();
		s_Data.TheShaderLibrary.Get("CircleShader")
			->SetMat4("u_ViewProjection", viewProj);
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;
		s_Data.CircleIndexCount = 0;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::BeginScene(const EditorCamera& camera) {
		PTC_PROFILE_FUNCTION();
		glm::mat4 viewProj = camera.GetViewProjection();

		s_Data.TheShaderLibrary.Get("QuadShader")->Bind();
		s_Data.TheShaderLibrary.Get("QuadShader")
			->SetMat4("u_ViewProjection", viewProj);
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.QuadIndexCount = 0;

		s_Data.TheShaderLibrary.Get("CircleShader")->Bind();
		s_Data.TheShaderLibrary.Get("CircleShader")
			->SetMat4("u_ViewProjection", viewProj);
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;
		s_Data.CircleIndexCount = 0;

		s_Data.TextureSlotIndex = 1;
	}


	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		PTC_PROFILE_FUNCTION();

		s_Data.TheShaderLibrary.Get("QuadShader")->Bind();
		s_Data.TheShaderLibrary.Get("QuadShader")
			->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.QuadIndexCount = 0;

		s_Data.TheShaderLibrary.Get("CircleShader")->Bind();
		s_Data.TheShaderLibrary.Get("CircleShader")
			->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;
		s_Data.CircleIndexCount = 0;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene() {
		PTC_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer2D::Flush() {
		if (s_Data.QuadIndexCount) {
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
			// Bind texture
			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) {
				s_Data.TextureSlots[i]->Bind(i);
			}
			s_Data.TheShaderLibrary.Get("QuadShader")->Bind();
			RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		}

		if (s_Data.CircleIndexCount) {
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
			s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);
			//// Bind texture
			//for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) {
			//	s_Data.TextureSlots[i]->Bind(i);
			//}
			s_Data.TheShaderLibrary.Get("CircleShader")->Bind();
			RenderCommand::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);
		}

	}

	void Renderer2D::DrawQuad(const glm::vec4& color, const glm::vec2& position, const float rotation, const glm::vec2& size) {
		DrawQuad(color, { position.x, position.y, 0.0f }, rotation, size);
	}

	void Renderer2D::DrawQuad(const glm::vec4& color, const glm::vec3& position, const float rotation, const glm::vec2& size) {
		PTC_PROFILE_FUNCTION();

		DrawQuad(color, TRS2Transform(position, { 0.0f, 0.0f, rotation }, { size, 1.0f }));
	}

	void Renderer2D::DrawQuad(const Ref<Texture2D>& tex, const glm::vec2& position, const float rotation, const glm::vec2& size, const float tilingFactor) {
		DrawQuad(tex, { position.x, position.y, 0.0f }, rotation, size, tilingFactor);
	}

	void Renderer2D::DrawQuad(const Ref<Texture2D>& tex, const glm::vec3& position, const float rotation, const glm::vec2& size, const float tilingFactor) {
		PTC_PROFILE_FUNCTION();

		DrawQuad(tex, TRS2Transform(position, { 0.0f, 0.0f, rotation }, { size, 1.0f }), tilingFactor);
	}

	void Renderer2D::DrawQuad(const Ref<SubTexture2D>& subtex, const glm::vec2& position, const float rotation, const glm::vec2& size, const float tilingFactor) {
		DrawQuad(subtex, { position.x, position.y, 0.0f }, rotation, size, tilingFactor);
	}

	void Renderer2D::DrawQuad(const Ref<SubTexture2D>& subtex, const glm::vec3& position, const float rotation, const glm::vec2& size, const float tilingFactor) {
		PTC_PROFILE_FUNCTION();

		DrawQuad(subtex, TRS2Transform(position, { 0.0f, 0.0f, rotation }, { size, 1.0f }), tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec4& color, const glm::mat4& transform, int entityID) {
		constexpr size_t quadVertexCount = 4;
		const float texIndex = 0.0f;	// whiteTexture
		constexpr glm::vec2 textureCoords[4] = { {0.0f, 0.0f}, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

		for (size_t i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}
		s_Data.QuadIndexCount += 6;
		s_Data.QuadCount++;
	}

	void Renderer2D::DrawQuad(const Ref<Texture2D>& tex, const glm::mat4& transform, const float tilingFactor, int entityID)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		constexpr glm::vec2 textureCoords[4] = { {0.0f, 0.0f}, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
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

		for (size_t i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.QuadCount++;
	}

	void Renderer2D::DrawQuad(const Ref<SubTexture2D>& subtex, const glm::mat4& transform, const float tilingFactor, int entityID) {
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		const glm::vec2* textureCoords = subtex->GetTexCoord();
		const Ref<Texture2D> texture = subtex->GetTexture();

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.QuadCount++;
	}

	void Renderer2D::DrawQuadRenderer(QuadRendererComponent& src, const glm::mat4& transform, int entityID) {
		DrawQuad(src.Color, transform, entityID);
	}


	void Renderer2D::DrawCircle(const glm::vec4& color, const glm::mat4& transform, float thickness, float fade, int entityID) {
		constexpr size_t circleVertexCount = 4;

		for (size_t i = 0; i < circleVertexCount; i++) {
			s_Data.CircleVertexBufferPtr->Position = transform * s_Data.CircleVertexPositions[i];
			s_Data.CircleVertexBufferPtr->LocalPosition = s_Data.CircleVertexPositions[i] * 2.0f;
			s_Data.CircleVertexBufferPtr->Color = color;
			s_Data.CircleVertexBufferPtr->Thickness = thickness;
			s_Data.CircleVertexBufferPtr->Fade = fade;
			s_Data.CircleVertexBufferPtr->EntityID = entityID;
			s_Data.CircleVertexBufferPtr++;
		}
		s_Data.CircleIndexCount += 6;
		s_Data.CircleCount++;
	}

	void Renderer2D::DrawCircleRenderer(CircleRendererComponent& src, const glm::mat4& transform, int entityID) {
		DrawCircle(src.Color, transform, src.Thickness, src.Fade, entityID);
	}


}