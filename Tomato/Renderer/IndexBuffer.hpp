#pragma once

#include "BufferLayout.hpp"

namespace Tomato
{
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		[[nodiscard]] virtual uint32_t GetCount() const = 0;
		static std::shared_ptr<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};
}
