#pragma once

#include <glm/vec2.hpp>
#include <vulkan/vulkan_core.h>

#include "VulkanCommandBuffer.h"
#include "ThirdParty/VulkanMemoryAllocator/include/vk_mem_alloc.h"
#include "Tomato/Core/Macro.h"
#include "Tomato/Renderer/Texture.h"

namespace Tomato
{
	struct VulkanImageInfo
	{
		VkImage Image = nullptr;
		VkImageView ImageView = nullptr;
		VkSampler Sampler = nullptr;
		VkDescriptorImageInfo DescriptorInfo{};
		VmaAllocation MemoryAlloc = nullptr;
	};

	class VulkanTexture2D : public Texture2D
	{
	public:
		VulkanTexture2D(const std::string& path, TextureProperties properties = TextureProperties());
		VulkanTexture2D(TextureFormat format, uint32_t width, uint32_t height, const void* data = nullptr,
		               TextureProperties properties = TextureProperties());
		~VulkanTexture2D() override;
		virtual void Resize(const glm::uvec2& size);
		virtual void Resize(uint32_t width, uint32_t height);

		void Invalidate();

		virtual TextureFormat GetFormat() const { return m_Format; }
		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }
		virtual glm::uvec2 GetSize() const { return {m_Width, m_Height}; }

		virtual void Bind(uint32_t slot = 0) const;

		const VkDescriptorImageInfo& GetVulkanDescriptorInfo() const
		{
			return m_image.DescriptorInfo;
		}

		void Lock();
		void Unlock();

		//Buffer GetWriteableBuffer() override;
		//bool Loaded() const  { return m_ImageData; }
		const std::string& GetPath() const override;
		uint32_t GetMipLevelCount() const;
		virtual std::pair<uint32_t, uint32_t> GetMipSize(uint32_t mip) const;

		void GenerateMips();


		uint32_t GetID() const override;


		void SetData(void* data, uint32_t size) override;


		void BindUnit(uint32_t slot = 0) const override;


		void Bind() const override;


		bool operator ==(const Texture& other) const override
		{
			throw std::logic_error("The method or operation is not implemented.");
		}


		TextureType GetType() const override;

	private:
		bool Load(const std::string& path);

	private:
		std::string m_Path;
		uint32_t m_Width;
		uint32_t m_Height;
		TextureProperties m_Properties;

		VulkanImageInfo m_image;

		TextureFormat m_Format = TextureFormat::None;
	};

	class VulkanTextureCube : public TextureCube
	{
	public:
		VulkanTextureCube(TextureFormat format, uint32_t width, uint32_t height, const void* data = nullptr,
		                  TextureProperties properties = TextureProperties());
		VulkanTextureCube(const std::string& path, TextureProperties properties = TextureProperties());
		void Release();
		~VulkanTextureCube() override;

		virtual void Bind(uint32_t slot = 0) const
		{
		}

		virtual TextureFormat GetFormat() const { return m_Format; }

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }
		virtual glm::uvec2 GetSize() const { return {m_Width, m_Height}; }

		virtual uint32_t GetMipLevelCount() const;
		virtual std::pair<uint32_t, uint32_t> GetMipSize(uint32_t mip) const;


		const VkDescriptorImageInfo& GetVulkanDescriptorInfo() const { return m_image.DescriptorInfo; }
		VkImageView CreateImageViewSingleMip(uint32_t mip);

		void GenerateMips(bool readonly = false);


		const std::string& GetPath() const override;


		uint32_t GetID() const override;


		void SetData(void* data, uint32_t size) override;


		void BindUnit(uint32_t slot = 0) const override;


		void Bind() const override;


		bool operator ==(const Texture& other) const override
		{
			throw std::logic_error("The method or operation is not implemented.");
		}


		TextureType GetType() const override;

	private:
		void Invalidate();

	private:
		TextureFormat m_Format = TextureFormat::None;
		uint32_t m_Width = 0, m_Height = 0;
		TextureProperties m_Properties;

		bool m_MipsGenerated = false;

		VulkanImageInfo m_image;
	};
}
