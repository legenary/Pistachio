#include "ptcpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Pistachio {
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, const glm::vec2& stride)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			PTC_CORE_ASSERT(false, "Renderer API none currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(width, height, stride);
		}
		PTC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path, const glm::vec2& stride) {

		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			PTC_CORE_ASSERT(false, "Renderer API none currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(path, stride);
		}
		PTC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

}