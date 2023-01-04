#pragma once

#include <string>
#include <vector>
#include <memory>

namespace Tomato {

	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float: return 4;
		case ShaderDataType::Float2:return 4 * 2;
		case ShaderDataType::Float3:return 4 * 3;
		case ShaderDataType::Float4:return 4 * 4;
		case ShaderDataType::Mat3:  return 4 * 3 * 3;
		case ShaderDataType::Mat4:  return 4 * 4 * 4;
		case ShaderDataType::Int:   return 4;
		case ShaderDataType::Int2:  return 4 * 2;
		case ShaderDataType::Int3:  return 4 * 3;
		case ShaderDataType::Int4:  return 4 * 4;
		case ShaderDataType::Bool:  return 1;
		default:break;
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

		BufferElement(ShaderDataType type, const std::string name, bool normalized = false)
			:Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(false) //size 和 offset先设置成0，以后支持
		{

		}

		[[nodiscard]] uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case ShaderDataType::Float	:return 1;
				case ShaderDataType::Float2	:return 2;
				case ShaderDataType::Float3	:return 3;
				case ShaderDataType::Float4	:return 4;
				case ShaderDataType::Mat3	:return 3 * 3;
				case ShaderDataType::Mat4	:return 4 * 4;
				case ShaderDataType::Int	:return 1;
				case ShaderDataType::Int2	:return 2;
				case ShaderDataType::Int3	:return 3;
				case ShaderDataType::Int4	:return 4;
				case ShaderDataType::Bool	:return 1;
				default:break;
			}
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			:m_elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		[[nodiscard]] uint32_t GetStride() const { return m_stride; }
		[[nodiscard]] const std::vector<BufferElement>& GetElements() const { return m_elements; }
		
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

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		[[nodiscard]] virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;	

		static std::shared_ptr<VertexBuffer> Create(float* vertices, uint32_t size);
		static std::shared_ptr<VertexBuffer> Create(uint32_t size);
	};

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