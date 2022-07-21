#pragma once

#include <string>
#include "Pistachio/Core/Core.h"
#include <glm/glm.hpp>

namespace Pistachio {

	class Texture {
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;
		
		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture {
	public:
		static Ref<Texture2D> Create(uint32_t width, uint32_t height, const glm::vec2& stride = {0, 0});
		static Ref<Texture2D> Create(const std::string& path, const glm::vec2& stride = { 0, 0 });

		virtual const glm::vec2& GetStride() const = 0;
	};
}


