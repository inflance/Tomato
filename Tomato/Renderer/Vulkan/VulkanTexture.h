#pragma once

#include "Tomato/Renderer/Texture.h"
#include "vulkan/vulkan_raii.hpp"

namespace Tomato
{
	struct Buffer;

	class VulkanTexture2D final : public Texture2D
	{
	public:
		VulkanTexture2D();

		VulkanTexture2D(const std::string& path);
		VulkanTexture2D(void* data, TextureInfo info);

		~VulkanTexture2D() override = default;

		void Init(void* data);

		void GenerateMipMap();

		[[nodiscard]] const std::string& GetPath() const override { return m_path; }
		void SetPath(const std::string& path) override { m_path = path; }

		void CreateImage(uint32_t width, uint32_t height);

		void AllocateImageMemory();

		void CreateImageView();

		void CreateSampler();

		void TransitionImageLayout(const vk::raii::CommandBuffer& cmdBuf, vk::ImageLayout lodLay,
		                           vk::ImageLayout newLay, vk::AccessFlagBits srcMask,
		                           vk::AccessFlagBits dstMask, vk::PipelineStageFlagBits srcPipMask,
		                           vk::PipelineStageFlagBits dstPipMask);
		void CopyDataToImage(vk::raii::CommandBuffer& cmd, const vk::raii::Buffer& buffer, uint32_t width,
		                     uint32_t height);

		[[nodiscard]] uint32_t GetWidth() const override { return m_info.extend_.width_; }


		[[nodiscard]] uint32_t GetHeight() const override { return m_info.extend_.height_; }


		[[nodiscard]] uint32_t GetID() const override { return 0; }


		void SetData(void* data, uint32_t size) override
		{
		}


		void BindUnit(uint32_t slot = 0) const override
		{
		}


		void Bind() const override
		{
		}

		bool operator ==(const Texture& other) const override
		{
			throw std::logic_error("The method or operation is not implemented.");
		}

		[[nodiscard]] TextureType GetType() const override { return TextureType::Texture2D; }

	private:
		std::string m_path;
		TextureInfo m_info;
		bool m_need_stage;

	public:
		vk::raii::Image image = nullptr;
		vk::Format format = vk::Format::eR8G8B8A8Unorm;
		vk::raii::DeviceMemory memory = nullptr;
		vk::raii::ImageView imageView = nullptr;
		vk::raii::Sampler sampler = nullptr;
	};
}
