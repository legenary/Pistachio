#pragma once

#include "Pistachio/Renderer/RendererAPI.h"

namespace Pistachio {

	class OpenGLRendererAPI : public RendererAPI {
	public:
		virtual void Clear() override;
		virtual void SetClearColor(const glm::vec4& color) override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;

	};

}