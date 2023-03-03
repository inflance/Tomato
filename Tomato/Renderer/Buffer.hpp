#pragma once
#include "Tomato/Core/Macro.h"

namespace Tomato
{
	struct Buffer
	{
		void* data_{};
		uint32_t size_{};
		uint32_t max_size_{};
		void* ptr_ = nullptr;

		Buffer()
			: data_(nullptr), size_(0)
		{
		}

		Buffer(void* data, uint32_t size)
			: data_(data), size_(size), max_size_(size)
		{
			ptr_ = data_;
			ptr_ = static_cast<byte*>(ptr_) + size;
		}

		template <typename T>
		void Allocate(const uint32_t size)
		{
			Free<T>();
			if (size == 0)
				return;
			data_ = new T[size];
			max_size_ = sizeof(T) * size;
			ptr_ = data_;
		}

		void Allocate(const uint32_t size)
		{
			Allocate<byte>(size);
		}

		template <typename T>
		[[nodiscard]] auto Size() const
		{
			return size_ / sizeof(T);
		}

		[[nodiscard]] auto Size() const
		{
			return Size<byte>();
		}

		template <typename T>
		void Free()
		{
			delete[] static_cast<T*>(data_);
			data_ = nullptr;
			size_ = 0;
			max_size_ = 0;
			ptr_ = nullptr;
		}

		void Free()
		{
			Free<byte>();
		}

		template <typename T>
		void Copy(const void* data, uint32_t size)
		{
			ASSERT(size_+ sizeof(T) * size < max_size_);
			memcpy(ptr_, data, sizeof(T) * size);
			ptr_ = static_cast<T*>(ptr_) + size;
			size_ += sizeof(T) * size;
		}

		void Copy(const void* data, uint32_t size)
		{
			Copy<byte>(data, size);
		}

		template<typename T>
		auto operator[](uint32_t index) const
		{
			return static_cast<T*>(data_)[index];
		}

		auto operator[](uint32_t index) const
		{
			return operator[]<byte>(index);
		}

		operator bool() const
		{
			return data_ != nullptr;
		}

		template <typename Ty>
		auto As() const
		{
			return static_cast<Ty*>(data_);
		}
	};
}
