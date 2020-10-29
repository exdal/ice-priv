#pragma once

#include "Utils/Math.h"
#include "Utils/Memory.h"

#include <bgfx/bgfx.h>
#include <bx/allocator.h>

#include <string>
#include <vector>

namespace IceSDK::Graphics {
    // Low level texture wrapper
    class Texture2D {
    public:
        ~Texture2D() {
            if (bgfx::isValid(this->_inner))
                bgfx::destroy(this->_inner); // Destroy GPU Texture Handle
        }

        static Memory::Ptr<Texture2D> Load(const std::string &pPath);
        static Memory::Ptr<Texture2D> Load(const std::string &pName, bool antiAliasing, uint8_t *data, const uint32_t dataSize);

        static uint8_t *LoadRawBitmap(uint8_t *data, uint32_t data_size, uint32_t *width, uint32_t *height);

        static Memory::Ptr<Texture2D> LoadRaw(
            const std::string &pName, size_t pWidth, size_t pHeight, bgfx::TextureFormat::Enum pTexFormat, const std::vector<uint8_t> &pData);

        static Memory::Ptr<Texture2D> Create(const std::string &pName, size_t pWidth, size_t pHeight, bgfx::TextureFormat::Enum pTexFormat, uint32_t flags = 0);

        uint32_t Width() const {
            return _width;
        }
        uint32_t Height() const {
            return _height;
        }

        void Modify(Math::Rectf pRect, const std::vector<uint8_t> &pBuffer, bgfx::TextureFormat::Enum pTexFormat);

        bgfx::TextureHandle GetHandle() const;
        bgfx::TextureHandle *GetHandlePtr();

        bool operator==(const Texture2D &other) {
            return _inner.idx == ((Texture2D &)other)._inner.idx;
        }

    private:
        uint32_t _width{};
        uint32_t _height{};

        bgfx::TextureHandle _inner = BGFX_INVALID_HANDLE;
    };
} // namespace IceSDK::Graphics
