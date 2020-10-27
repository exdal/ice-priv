#pragma once

#include "Assets/Asset.h"

#include "Utils/Memory.h"

#include "Graphics/Shaders/ShaderManager.h"

#include <bgfx/bgfx.h>

#include <vector>

namespace IceSDK::Assets {
    class ShaderAsset {
    public:
        // Memory::Ptr<Audio::Audio> LoadAudio() const;

        constexpr static eAssetType GetAssetType() {
            return eAssetType::Shader;
        }

        uint8_t *Data() const;
        uint32_t DataSize() const;
        
        static ShaderAsset From(std::string_view _name, uint8_t *_data, const uint32_t _data_size);

    private:
        struct Shader {
            Graphics::Shaders::eShaderType type;
            bgfx::RendererType::Enum platform;
            std::vector<uint8_t> code;
        };

        std::vector<Shader> _shaders{};
    };
} // namespace IceSDK::Assets
