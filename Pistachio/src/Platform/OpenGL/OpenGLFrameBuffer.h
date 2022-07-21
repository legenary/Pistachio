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

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }
		virtual uint32_t GetDepthAttachmentRendererID() const override { return m_DepthAttachment; }

		virtual const FrameBufferSpecification& GetSpecification() const override { return m_Specification; };

	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment, m_DepthAttachment;
		FrameBufferSpecification m_Specification;


	};


}