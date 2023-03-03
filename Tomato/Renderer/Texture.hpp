#pragma once

#include "Tomato/Core/Structs.h"

namespace Tomato
{
	enum class CullMode : int32_t
	{
		Front,
		Back,
		FrontAndBack,
		None
	};

	enum class PolygonMode : int32_t
	{
		Fill,
		Line,
		Point
	};

	enum class BlendMode : uint32_t
	{
		None = 0,
		OneZero,
		ZeroSrcColor,
		SrcAlphaOneMinusSrcAlpha,
		Add
	};

	enum class TextureWrap : uint32_t
	{
		None,
		Repeat,
		Clamp,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder
	};

	enum class TextureFilter : uint32_t
	{
		None,
		Linear,
		Nearest,
		Cubic
	};


	enum class TextureFormat : uint32_t
	{
		None = 0,
		R8,
		R8UI,
		R16,
		R16UI,
		R16F,
		R32F,
		R32I,
		R32UI,
		RG8,
		RG16F,
		RG32F,
		RGB8,
		RGBA8,
		RGB16F,
		RGBA16F,
		RGB32,
		RGBA32,
		RGBA32F,
		RGB,
		RGBA,
		Color = RGBA,
		B10R11G11UF,
		Stencil,
		DEPTH32FSTENCIL8UI,
		DEPTH24STENCIL8,

		//by default 
		Depth = DEPTH24STENCIL8,
	};

	enum class TextureUsage
	{
		None = 0,
		Color,
		Depth,
		Storage
	};

	enum class TextureType : uint32_t
	{
		None = 0,
		Color,
		Array,
		Depth,
		Cube
	};

	struct TextureInfo
	{
		std::string debug_name_;
		TextureFormat format_ = TextureFormat::RGBA;
		TextureUsage usage_ = TextureUsage::Color;
		TextureWrap sampler_wrap_ = TextureWrap::Repeat;
		TextureFilter sampler_filter_ = TextureFilter::Linear;
		TextureType type_ = TextureType::Color;
		Extend3 extend_{500, 500, 1};
		uint32_t mip_levels_ = 1;
		uint32_t layers_{};

		std::optional<uint32_t> multi_sampler_rate;
		bool gen_mips_ = true;
		bool srgb_ = false;
		bool stage_ = true;
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		[[nodiscard]] virtual const std::string& GetPath() const = 0;
		virtual void SetPath(const std::string& path) = 0;
		virtual void SetInfo(const TextureInfo& info) = 0;
		[[nodiscard]] virtual uint32_t GetWidth() const = 0;
		[[nodiscard]] virtual uint32_t GetHeight() const = 0;
		[[nodiscard]] virtual uint32_t GetID() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void BindUnit(uint32_t slot = 0) const = 0;
		virtual void Bind() const = 0;
		[[nodiscard]] virtual const void* GetDescriptorInfo() const = 0;
		virtual bool operator==(const Texture& other) const = 0;
		[[nodiscard]] virtual TextureType GetType() const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static std::shared_ptr<Texture2D> Create(const std::string& path,
		                                         TextureInfo info = TextureInfo());
		static std::shared_ptr<Texture2D> Create(void* data, TextureInfo info);
		static std::shared_ptr<Texture2D> Create(TextureInfo info);
	};

	class TextureCube : public Texture
	{
	public:
		static std::shared_ptr<TextureCube> Create(const std::string& path, TextureInfo info);
	};
}
