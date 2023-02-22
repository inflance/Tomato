#pragma once

#include <string>
#include <vector>

namespace Tomato
{
	enum class ShaderDataType
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float: return 4;
		case ShaderDataType::Float2: return 4 * 2;
		case ShaderDataType::Float3: return 4 * 3;
		case ShaderDataType::Float4: return 4 * 4;
		case ShaderDataType::Mat3: return 4 * 3 * 3;
		case ShaderDataType::Mat4: return 4 * 4 * 4;
		case ShaderDataType::Int: return 4;
		case ShaderDataType::Int2: return 4 * 2;
		case ShaderDataType::Int3: return 4 * 3;
		case ShaderDataType::Int4: return 4 * 4;
		case ShaderDataType::Bool: return 1;
		case ShaderDataType::None: ;
		default: break;
		}
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		BufferElement() = default;

		BufferElement(ShaderDataType type, std::string name, bool normalized = false)
			: Name(std::move(name)), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(false)
		{
		}

		[[nodiscard]] uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float: return 1;
			case ShaderDataType::Float2: return 2;
			case ShaderDataType::Float3: return 3;
			case ShaderDataType::Float4: return 4;
			case ShaderDataType::Mat3: return 3 * 3;
			case ShaderDataType::Mat4: return 4 * 4;
			case ShaderDataType::Int: return 1;
			case ShaderDataType::Int2: return 2;
			case ShaderDataType::Int3: return 3;
			case ShaderDataType::Int4: return 4;
			case ShaderDataType::Bool: return 1;
			case ShaderDataType::None: ;
			default: break;
			}
			return 0;
		}
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout() = default;

		VertexBufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		[[nodiscard]] uint32_t GetStride() const { return m_stride; }
		[[nodiscard]] const std::vector<BufferElement>& GetElements() const { return m_elements; }
		uint32_t GetElementCount() const { return m_elements.size(); }

		std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_elements.end(); }
		[[nodiscard]] std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		[[nodiscard]] std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }

	private:
		void CalculateOffsetsAndStride()
		{
			uint32_t offset = 0;
			m_stride = 0;

			for (auto& element : m_elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_stride += element.Size;
			}
		}

	private:
		std::vector<BufferElement> m_elements;
		uint32_t m_stride = 0;
	};

	enum class DescriptorType : uint32_t
	{
		UniformBuffer = 0,
		StorageBuffer,
		CombinedImageSample
	};

	enum class ShaderStageFlags : uint32_t
	{
		Fragment,
		Vertex,
	};

	struct UniformBufferSetElement
	{
		DescriptorType Type = DescriptorType::UniformBuffer;
		uint32_t Count{};
		ShaderStageFlags Flag = ShaderStageFlags::Fragment;
		uint32_t Binding = 0;
		uint32_t Set = 0;
	};

	class UniformBufferSetLayout
	{
	public:
		UniformBufferSetLayout(const std::initializer_list<UniformBufferSetElement>& elements)
			: m_elements(elements)
		{
		}

		[[nodiscard]] const std::vector<UniformBufferSetElement>& GetElements() const { return m_elements; }

		auto operator[](uint32_t index) const
		{
			return m_elements[index];
		}

		[[nodiscard]] uint32_t Size() const { return m_elements.size(); }
		std::vector<UniformBufferSetElement>::iterator begin() { return m_elements.begin(); }
		std::vector<UniformBufferSetElement>::iterator end() { return m_elements.end(); }
		[[nodiscard]] std::vector<UniformBufferSetElement>::const_iterator begin() const { return m_elements.begin(); }
		[[nodiscard]] std::vector<UniformBufferSetElement>::const_iterator end() const { return m_elements.end(); }

	private:
		std::vector<UniformBufferSetElement> m_elements;
	};
}
