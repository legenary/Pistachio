#include "ptcpch.h"

#include "FrameBuffer.h"

#include "Pistachio/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Pistachio {

	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			PTC_CORE_ASSERT(false, "Renderer API none currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFrameBuffer>(spec);
		}
		PTC_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}




}