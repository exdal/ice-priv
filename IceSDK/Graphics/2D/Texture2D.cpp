#include "Texture2D.h"

#include "bimg/decode.h"
#include "bgfx/bgfx.h"
#include "Utils/FileSystem.h"

using namespace IceSDK;
using namespace IceSDK::Graphics;

static void DeleteImageContainer(void *pPtr, void *pUserData)
{
	auto *const imageContainer = static_cast<bimg::ImageContainer *>(pUserData);
	bimg::imageFree(imageContainer);
}

Memory::WeakPtr<Texture2D> Texture2D::Load(const Memory::WeakPtr<bx::AllocatorI> &pBxAlloc, const std::string &pPath)
{
	return Texture2D::Load(pBxAlloc, FileSystem::GetFileName(pPath), FileSystem::ReadBinaryFile(pPath));
}

Memory::WeakPtr<Texture2D> Texture2D::Load(Memory::WeakPtr<bx::AllocatorI> pBxAlloc, const std::string &pName, const std::vector<uint8_t> &pData)
{
	const auto bxAlloc = pBxAlloc.lock();
	if (bxAlloc)
		return {};

	bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;

	Memory::WeakPtr<Texture2D> tex2D = std::make_shared<Texture2D>(pBxAlloc);

	auto p_Tex2D = tex2D.lock();
	auto *p_ImageContainer = bimg::imageParse(bxAlloc.get(), pData.data(), pData.size());
	if (p_ImageContainer == nullptr)
		return {};

	const auto *const mem = bgfx::makeRef(
		p_ImageContainer->m_data,
		p_ImageContainer->m_size,
		DeleteImageContainer,
		p_ImageContainer);

	if (!bgfx::isTextureValid(0, false, p_ImageContainer->m_numLayers, bgfx::TextureFormat::Enum(p_ImageContainer->m_format), bgfx::TextureFormat::RGBA4))
		return {};

	p_Tex2D->_inner = bgfx::createTexture2D(
		static_cast<uint16_t>(p_ImageContainer->m_width),
		static_cast<uint16_t>(p_ImageContainer->m_height),
		1 < p_ImageContainer->m_numMips,
		p_ImageContainer->m_numLayers,
		static_cast<bgfx::TextureFormat::Enum>(p_ImageContainer->m_format),
		bgfx::TextureFormat::RGBA4,
		mem);

	if (!bgfx::isValid(p_Tex2D->_inner))
		return {};

	bgfx::setName(p_Tex2D->_inner, pName.data(), pName.length());

	return tex2D;
}
