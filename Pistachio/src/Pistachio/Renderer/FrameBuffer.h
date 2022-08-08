#pragma once

#include "Pistachio/Core/Core.h"

namespace Pistachio {

	enum class FramebufferTextureFormat {
		None = 0,

		//Color
		RGBA8,
		RED_INTEGER,

		// Depth/stencil
		DEPTH24STENDIL8,

		// defaults
		Depth = DEPTH24STENDIL8

	};

	struct FramebufferTextureSpecification {
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
	};

	struct FramebufferAttachmentSpecification {
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachements)
			: Attachments(attachements) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FrameBufferSpecification {
		uint32_t Width = 1, Height = 1;
		FramebufferAttachmentSpecification Attachments;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
		// glBindFrameBuffer(0); // unbind all the frame buffer
	};

	class FrameBuffer {

	public:
		virtual ~FrameBuffer() = default;

		//virtual FrameBufferSpecification& GetSpecification() = 0;
		virtual const FrameBufferSpecification& GetSpecification() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0; // temp

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
		virtual uint32_t GetDepthAttachmentRendererID() const = 0;

		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);

	};


}