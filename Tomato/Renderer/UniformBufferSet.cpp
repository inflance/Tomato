#include "UniformBufferSet.hpp"

#include "Tomato/Renderer/Texture.hpp"
#include "Tomato/Renderer/UniformBuffer.hpp"

namespace Tomato
{
	bool UniformBufferSet::FindTexture(Binding binding, Set set)
	{
		return m_texture_set.contains(set) && m_texture_set[set].contains(binding);
	}

	Ref<Texture2D> UniformBufferSet::GetTexture(Binding binding, Set set)
	{
		ASSERT(FindTexture(binding, set));
		return m_texture_set[set][binding];
	}

	void UniformBufferSet::SetTexture(const Ref<Texture2D>& texture, Binding binding, Set set)
	{
		m_texture_set[set][binding] = texture;
	}

	bool UniformBufferSet::FindUniformBuffer(Binding binding, Set set)
	{
		return m_uniform_set.contains(set) && m_uniform_set[set].contains(binding);
	}

	Ref<UniformBuffer> UniformBufferSet::GetUniformBuffer(Binding binding, Set set)
	{
		ASSERT(FindUniformBuffer(binding, set));
		return m_uniform_set[set][binding];
	}

	void UniformBufferSet::SetUniformBuffer(Ref<UniformBuffer> uniformBuffer, Set set)
	{
		m_uniform_set[set][uniformBuffer->GetBinding()] = uniformBuffer;
	}

	void UniformBufferSet::SetUniformBufferSetLayout(const UniformBufferSetLayout& layout)
	{
		m_layout = layout;
	}
}
