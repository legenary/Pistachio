#include "ptcpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"


namespace Pistachio {
	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			PTC_CORE_ASSERT(false, "Renderer API none currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vertexSrc, fragmentSrc);
		}
		PTC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}
