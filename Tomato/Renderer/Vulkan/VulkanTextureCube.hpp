#pragma once

#include "../Texture.hpp"
#include "Tomato/Renderer/Buffer.hpp"
#include "vulkan/vulkan_raii.hpp"

namespace Tomato
{
	class VulkanTextureCube : public TextureCube
	{
	public:
		VulkanTextureCube();
	
		VulkanTextureCube(Buffer buffer, TextureInfo info);
		~VulkanTextureCube() override;

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

		[[nodiscard]] const std::string& GetPath() const override { return m_path; };
		void SetPath(const std::string& path) override;
		void SetInfo(const TextureInfo& info) override;
		[[nodiscard]] uint32_t GetWidth() const override;
		[[nodiscard]] uint32_t GetHeight() const override;
		[[nodiscard]] uint32_t GetID() const override;
		void SetData(void* data, uint32_t size) override;
		void BindUnit(uint32_t slot) const override;
		void Bind() const override;
		bool operator==(const Texture& other) const override;
		[[nodiscard]] TextureType GetType() const override { return TextureType::Cube; }


		[[nodiscard]] const auto& GetImageView() const { return imageView; }

		[[nodiscard]] const void* GetDescriptorInfo() const override { return &descriptor; }

		void UpdateDescriptor();
	private:
	private:
		std::string m_path;
		TextureInfo m_info;
		bool m_need_stage{ false };

	public:
		vk::raii::Image image = nullptr;
		vk::Format format = vk::Format::eR8G8B8A8Unorm;
		vk::ImageLayout layout{};
		vk::SampleCountFlagBits sample_count = vk::SampleCountFlagBits::e1;
		vk::raii::DeviceMemory memory = nullptr;
		vk::raii::ImageView imageView = nullptr;
		vk::raii::Sampler sampler = nullptr;
		vk::DescriptorImageInfo descriptor;
	};
}
