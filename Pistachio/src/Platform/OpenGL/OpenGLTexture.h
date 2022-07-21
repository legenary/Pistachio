#pragma once

#include "Pistachio/Renderer/Texture.h"
#include <glad/glad.h>

namespace Pistachio {

	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height, const glm::vec2& stride);
		OpenGLTexture2D(const std::string& path, const glm::vec2& stride);
		virtual ~OpenGLTexture2D();

		inline virtual uint32_t GetWidth() const override { return m_Width; }
		inline virtual uint32_t GetHeight() const override { return m_Height; }
		inline virtual const glm::vec2& GetStride() const override { return m_Stride; }


		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override {
			return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
		}
	private:
		std::string m_Path;
		uint32_t m_RendererID;
		uint32_t m_Width, m_Height;
		glm::vec2 m_Stride;
		GLenum m_InternalFormat, m_DataFormat;

	};

}



