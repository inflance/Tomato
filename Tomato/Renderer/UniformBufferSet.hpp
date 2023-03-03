#pragma once

#include <map>

#include "BufferLayout.hpp"
#include "Tomato/Core/Macro.h"

namespace Tomato
{
	class UniformBuffer;
	class Texture2D;
	using Set = uint32_t;
	using Binding = uint32_t;

	class UniformBufferSet
	{
	public:
		UniformBufferSet() = default;
		virtual ~UniformBufferSet() = default;

		bool FindTexture(Binding binding, Set set = 0);

		Ref<Texture2D> GetTexture(Binding binding, Set set = 0);

		void SetTexture(const Ref<Texture2D>& texture, Binding binding = 0, Set set = 0);

		bool FindUniformBuffer(Binding binding, Set set = 0);

		Ref<UniformBuffer> GetUniformBuffer(Binding binding, Set set = 0);

		void SetUniformBuffer(Ref<UniformBuffer> uniformBuffer, Set set = 0);

		void SetUniformBufferSetLayout(const UniformBufferSetLayout& layout);

		[[nodiscard]]const auto& GetUniformBufferSetLayout() const { return m_layout; }

	private:
		std::map<Set, std::map<Binding, Ref<UniformBuffer>>> m_uniform_set;
		std::map<Set, std::map<Binding, Ref<Texture2D>>> m_texture_set;
		UniformBufferSetLayout m_layout{};
	};
}
