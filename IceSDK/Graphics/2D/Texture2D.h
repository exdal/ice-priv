#pragma once

#include <string>
#include <vector>

#include "Utils/Memory.h"

#include "bgfx/bgfx.h"
#include "bimg/bimg.h"
#include "bx/allocator.h"

namespace IceSDK
{
	namespace Graphics
	{
		// Low level texture wrapper
		class Texture2D
		{
		public:
			explicit Texture2D(Memory::WeakPtr<bx::AllocatorI> pBxAlloc)
				: _bxAlloc(pBxAlloc)
			{
			}

			~Texture2D()
			{
				auto bxAlloc = this->_bxAlloc.lock();

				bgfx::destroy(this->_inner); // Destroy GPU Texture Handle
			}

			static Memory::WeakPtr<Texture2D> Load(const Memory::WeakPtr<bx::AllocatorI> &pBxAlloc, const std::string &pPath);
			static Memory::WeakPtr<Texture2D> Load(Memory::WeakPtr<bx::AllocatorI> pBxAlloc, const std::string &pName, const std::vector<uint8_t> &pData);

		private:
			std::weak_ptr<bx::AllocatorI> _bxAlloc{};

			bgfx::TextureHandle _inner = BGFX_INVALID_HANDLE;
		};
	} // namespace Graphics
} // namespace IceSDK
