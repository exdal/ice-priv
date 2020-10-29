#include "pch.h"

#include "ECS/Components/BaseComponent.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Scene.h"

#include "Utils/Instrumentor.h"

#include "Graphics/Components/ShaderComponent.h"
#include "Graphics/Components/SpriteComponent.h"
#include "Graphics/Components/TileComponent.h"
#include "Graphics/EntityHelper.h"
#include "Graphics/Shaders/compiled/fs_sprite.d3d11.h"
#include "Graphics/Shaders/compiled/fs_sprite.d3d12.h"
#include "Graphics/Shaders/compiled/fs_sprite.d3d9.h"
#include "Graphics/Shaders/compiled/fs_sprite.glsl.h"
#include "Graphics/Shaders/compiled/fs_sprite.metal.h"
#include "Graphics/Shaders/compiled/fs_sprite.vulkan.h"
#include "Graphics/Shaders/compiled/vs_sprite.d3d11.h"
#include "Graphics/Shaders/compiled/vs_sprite.d3d12.h"
#include "Graphics/Shaders/compiled/vs_sprite.d3d9.h"
#include "Graphics/Shaders/compiled/vs_sprite.glsl.h"
#include "Graphics/Shaders/compiled/vs_sprite.metal.h"
#include "Graphics/Shaders/compiled/vs_sprite.vulkan.h"
#include "Graphics/Systems/SpriteRenderingSystem.h"

using namespace IceSDK;
using namespace IceSDK::Graphics;
using namespace IceSDK::Graphics::Entity;

IceSDK::Entity Graphics::Entity::CreateSprite(Memory::Ptr<IceSDK::Scene> pScene, Memory::Ptr<Shaders::ShaderManager> pShaderManager, Memory::Ptr<Texture2D> pTex,
    const glm::vec3 &pPosition, const glm::vec2 &pSize, const glm::vec4 &pTileinfo, float pRotation) {
    ICESDK_PROFILE_FUNCTION();

    auto entity = pScene->CreateEntity("Sprite");

    glm::vec2 TexSize{ 0 };
    if (pSize.x != -1.f && pSize.y != -1.f)
        TexSize = pSize;
    else if (pTex != nullptr)
        TexSize = { pTex->Width(), pTex->Height() };

    entity.AddComponent<IceSDK::Components::TransformComponent>(pPosition, glm::vec3{ 1.0f, 1.0f, 1.0f }, pRotation);

    entity.AddComponent<Graphics::Components::SpriteComponent>(TexSize, pTex);

    if (pTileinfo.x != -1.f && pTileinfo.y != -1.f) {
        entity.AddComponent<Graphics::Components::TileComponent>(pTileinfo);
    }

    return entity;
}

void Graphics::Entity::AttachAnimation(IceSDK::Entity &entity, IceSDK::Graphics::Components::AnimationComponent anim_component) {
    entity.AddComponent<Graphics::Components::AnimationComponent>(anim_component);
}

void Graphics::Entity::Init(const Memory::Ptr<Graphics::Shaders::ShaderManager> &pShaderManager) {
    ICESDK_PROFILE_FUNCTION();

    pShaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Direct3D9, Shaders::eShaderType::Fragment, std::vector<uint8_t>(&fs_sprite_d3d9[0], &fs_sprite_d3d9[sizeof fs_sprite_d3d9]));
    pShaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Direct3D11, Shaders::eShaderType::Fragment, std::vector<uint8_t>(&fs_sprite_d3d11[0], &fs_sprite_d3d11[sizeof fs_sprite_d3d11]));
    pShaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Direct3D12, Shaders::eShaderType::Fragment, std::vector<uint8_t>(&fs_sprite_d3d12[0], &fs_sprite_d3d12[sizeof fs_sprite_d3d12]));
    pShaderManager->AppendShader(
        "Sprite", bgfx::RendererType::OpenGL, Shaders::eShaderType::Fragment, std::vector<uint8_t>(&fs_sprite_glsl[0], &fs_sprite_glsl[sizeof fs_sprite_glsl]));
    pShaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Metal, Shaders::eShaderType::Fragment, std::vector<uint8_t>(&fs_sprite_metal[0], &fs_sprite_metal[sizeof fs_sprite_metal]));
    pShaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Vulkan, Shaders::eShaderType::Fragment, std::vector<uint8_t>(&fs_sprite_vulkan[0], &fs_sprite_vulkan[sizeof fs_sprite_vulkan]));

    pShaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Direct3D9, Shaders::eShaderType::Vertex, std::vector<uint8_t>(&vs_sprite_d3d9[0], &vs_sprite_d3d9[sizeof vs_sprite_d3d9]));
    pShaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Direct3D11, Shaders::eShaderType::Vertex, std::vector<uint8_t>(&vs_sprite_d3d11[0], &vs_sprite_d3d11[sizeof vs_sprite_d3d11]));
    pShaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Direct3D12, Shaders::eShaderType::Vertex, std::vector<uint8_t>(&vs_sprite_d3d12[0], &vs_sprite_d3d12[sizeof vs_sprite_d3d12]));
    pShaderManager->AppendShader(
        "Sprite", bgfx::RendererType::OpenGL, Shaders::eShaderType::Vertex, std::vector<uint8_t>(&vs_sprite_glsl[0], &vs_sprite_glsl[sizeof vs_sprite_glsl]));
    pShaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Metal, Shaders::eShaderType::Vertex, std::vector<uint8_t>(&vs_sprite_metal[0], &vs_sprite_metal[sizeof vs_sprite_metal]));
    pShaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Vulkan, Shaders::eShaderType::Vertex, std::vector<uint8_t>(&vs_sprite_vulkan[0], &vs_sprite_vulkan[sizeof vs_sprite_vulkan]));
}

void Graphics::Entity::InitScene(const Memory::Ptr<IceSDK::Scene> &pScene) {
    ICESDK_PROFILE_FUNCTION();

    pScene->RegisterSystem<Systems::SpriteRenderingSystem>();
}
