#include "VulkanTexture.h"

namespace Tomato
{
	

	VulkanTextureCube::VulkanTextureCube(TextureFormat format, uint32_t width, uint32_t height, const void* data, TextureProperties properties)
	{

	}

	VulkanTextureCube::VulkanTextureCube(const std::string& path, TextureProperties properties)
	{

	}

	void VulkanTextureCube::Release()
	{

	}

	VulkanTextureCube::~VulkanTextureCube()
	{

	}

	uint32_t VulkanTextureCube::GetMipLevelCount() const
	{
		throw std::runtime_error("");
	}

	std::pair<uint32_t, uint32_t> VulkanTextureCube::GetMipSize(uint32_t mip) const
	{
		throw std::runtime_error("");
	}

	VkImageView VulkanTextureCube::CreateImageViewSingleMip(uint32_t mip)
	{
		throw std::runtime_error("");
	}

	void VulkanTextureCube::GenerateMips(bool readonly /*= false*/)
	{

	}

	const std::string& VulkanTextureCube::GetPath() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	uint32_t VulkanTextureCube::GetID() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void VulkanTextureCube::SetData(void* data, uint32_t size)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void VulkanTextureCube::BindUnit(uint32_t slot /*= 0*/) const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void VulkanTextureCube::Bind() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	Tomato::TextureType VulkanTextureCube::GetType() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void VulkanTextureCube::Invalidate()
	{

	}

	VulkanTexture2D::VulkanTexture2D(const std::string& path, TextureProperties properties)
	{

	}

	VulkanTexture2D::VulkanTexture2D(TextureFormat format, uint32_t width, uint32_t height, const void* data, TextureProperties properties)
	{

	}

	VulkanTexture2D::~VulkanTexture2D()
	{

	}

	void VulkanTexture2D::Resize(const glm::uvec2& size)
	{

	}

	void VulkanTexture2D::Resize(uint32_t width, uint32_t height)
	{

	}

	void VulkanTexture2D::Invalidate()
	{

	}

	void VulkanTexture2D::Bind(uint32_t slot /*= 0*/) const
	{

	}

	void VulkanTexture2D::Bind() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	Tomato::TextureType VulkanTexture2D::GetType() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void VulkanTexture2D::Lock()
	{

	}

	void VulkanTexture2D::Unlock()
	{

	}

	const std::string& VulkanTexture2D::GetPath() const
	{
		throw std::runtime_error("");
	}

	uint32_t VulkanTexture2D::GetMipLevelCount() const
	{
		throw std::runtime_error("");
	}

	std::pair<uint32_t, uint32_t> VulkanTexture2D::GetMipSize(uint32_t mip) const
	{
		throw std::runtime_error("");
	}

	void VulkanTexture2D::GenerateMips()
	{

	}

	uint32_t VulkanTexture2D::GetID() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void VulkanTexture2D::SetData(void* data, uint32_t size)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void VulkanTexture2D::BindUnit(uint32_t slot /*= 0*/) const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	bool VulkanTexture2D::Load(const std::string& path)
	{
		throw std::runtime_error("");
	}

}