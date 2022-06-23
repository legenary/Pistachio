#include "ptcpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Pistachio {
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			PTC_CORE_ASSERT(false, "Renderer API none currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexArray>();
		}
		PTC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}