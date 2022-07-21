#pragma once

#include "Pistachio/Core/Core.h"

namespace Pistachio {

	struct FrameBufferSpecification {
		uint32_t Width, Height;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
		// glBindFrameBuffer(0); // unbind all the frame buffer
	};

	class FrameBuffer {

	public:
		//virtual FrameBufferSpecification& GetSpecification() = 0;
		virtual const FrameBufferSpecification& GetSpecification() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;
		virtual uint32_t GetDepthAttachmentRendererID() const = 0;

		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);

	};


}