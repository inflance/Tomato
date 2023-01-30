#pragma once

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
		Nearest
	};

	enum class TextureFormat : uint32_t
	{
		None = 0,
		R8,
		R16,
		R16F,
		R32F,
		R32I,
		R32UI,
		RG8,
		RG16F,
		RGB8,
		RGBA8,
		RGB16,
		RGBA16,
		RGB32,
		RGBA32,
		RGB,
		RGBA,
		R11G11B10,
		DEPTH,
		STENCIL,
		DEPTH_STENCIL,
		SCREEN,
		LENGTH
	};

	enum class TextureType : uint32_t
	{
		None = 0,
		Texture2D,
		TextureCube
	};

	struct TextureProperties
	{
		std::string DebugName;
		TextureWrap SamplerWrap = TextureWrap::Repeat;
		TextureFilter SamplerFilter = TextureFilter::Linear;
		bool GenerateMips = true;
		bool SRGB = false;
		bool Storage = false;
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual const std::string& GetPath() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetID() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void BindUnit(uint32_t slot = 0) const = 0;
		virtual void Bind() const = 0;

		virtual bool operator==(const Texture& other) const = 0;
		virtual TextureType GetType() const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static std::shared_ptr<Texture2D> Create(const std::string& path,
		                                         TextureProperties properties = TextureProperties());
		static std::shared_ptr<Texture2D> Create(TextureFormat formmat, uint32_t width, uint32_t height);
	};

	class TextureCube : public Texture
	{
	public:
		static std::shared_ptr<TextureCube> Create(const std::string& path, TextureProperties properties);
		static std::shared_ptr<TextureCube> Create(TextureFormat formmat, uint32_t width, uint32_t height);
	};
}
