#pragma once


namespace Tomato
{
	class VertexBufferLayout;

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;


		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

		[[nodiscard]] virtual const VertexBufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const VertexBufferLayout& layout) = 0;

		static std::shared_ptr<VertexBuffer> Create(float* vertices, uint32_t size);
		static std::shared_ptr<VertexBuffer> Create(uint32_t size);
	};
}
