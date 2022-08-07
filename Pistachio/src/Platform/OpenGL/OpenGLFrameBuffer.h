#pragma once

#include "Pistachio/Renderer/FrameBuffer.h"

namespace Pistachio {

	class OpenGLFrameBuffer : public FrameBuffer {

	public:

		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		void Invalidate();	// resize

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index) const override { PTC_CORE_ASSERT(index < m_ColorAttachments.size(), "Exceeds maximum color attachment 4."); return m_ColorAttachments[index]; }
		virtual uint32_t GetDepthAttachmentRendererID() const override { return m_DepthAttachment; }

		virtual const FrameBufferSpecification& GetSpecification() const override { return m_Specification; };

	private:
		uint32_t m_RendererID = 0;
		FrameBufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecs;
		FramebufferTextureSpecification m_DepthAttachmentSpec = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 1;
	};


}