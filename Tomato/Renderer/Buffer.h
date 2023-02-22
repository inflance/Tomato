#pragma once
#include "Tomato/Core/Macro.h"

namespace Tomato
{
	struct Buffer
	{
		void* data_{};
		uint32_t size_{};

		Buffer()
			: data_(nullptr), size_(0)
		{
		}

		Buffer(void* data, uint32_t size)
			: data_(data), size_(size)
		{
		}

		void Allocate(const uint32_t size)
		{
			Free();
			if (size == 0)
				return;
			data_ = new byte[size];
			size_ = size;
		}

		void Free()
		{
			delete[] static_cast<byte*>(data_);
			data_ = nullptr;
			size_ = 0;
		}

		void Copy(void* data, uint32_t size) const
		{
			memcpy(data_, data, size);
		}

		auto operator[](uint32_t index) const
		{
			return static_cast<byte*>(data_)[index];
		}

		operator bool() const
		{
			return data_ != nullptr;
		}

		template <typename T>
		auto As() const
		{
			return static_cast<T*>(data_);
		}
	};
}
