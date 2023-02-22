#pragma once
#include <map>

#include "BufferLayout.h"
#include "Texture.h"
#include "Tomato/Core/Macro.h"
#include "UniformBuffer.h"

namespace Tomato
{
	using Set = uint32_t;
	using Binding = uint32_t;

	class UniformBufferSet
	{
	public:
		UniformBufferSet() = default;
		virtual ~UniformBufferSet() = default;

		bool FindTexture(Binding binding, Set set = 0)
		{
			return m_texture_set.contains(set) && m_texture_set[set].contains(binding);
		}

		Ref<Texture2D> GetTexture(Binding binding, Set set = 0)
		{
			ASSERT(FindTexture(binding, set));
			return m_texture_set[set][binding];
		}

		void SetTexture(const Ref<Texture2D>& texture, Binding binding = 0, Set set = 0)
		{
			m_texture_set[set][binding] = texture;
		}


		bool FindUniformBuffer(Binding binding, Set set = 0)
		{
			return m_uniform_set.contains(set) && m_uniform_set[set].contains(binding);
		}

		Ref<UniformBuffer> GetUniformBuffer(Binding binding, Set set = 0)
		{
			ASSERT(FindUniformBuffer(binding, set));
			return m_uniform_set[set][binding];
		}

		void SetUniformBuffer(Ref<UniformBuffer> uniformBuffer, Set set = 0)
		{
			m_uniform_set[set][uniformBuffer->GetBinding()] = uniformBuffer;
		}

		void SetUniformBufferSetLayout(const UniformBufferSetLayout& layout)
		{
			m_layout = layout;
		}

		const auto& GetUniformBufferSetLayout() const { return m_layout; }

	private:
		std::map<Set, std::map<Binding, Ref<UniformBuffer>>> m_uniform_set;
		std::map<Set, std::map<Binding, Ref<Texture2D>>> m_texture_set;
		UniformBufferSetLayout m_layout{};
	};
}
