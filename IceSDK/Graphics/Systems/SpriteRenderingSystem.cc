#include "pch.h"

#include "Graphics/Systems/SpriteRenderingSystem.h"

#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Entity.h"
#include "ECS/Systems/CameraSystem.h"

#include "Utils/Instrumentor.h"

#include "GameBase.h"
#include "Graphics/Components/AnimationComponent.h"
#include "Graphics/Components/ShaderComponent.h"
#include "Graphics/Components/SpriteComponent.h"
#include "Graphics/Components/TileComponent.h"

using namespace IceSDK;
using namespace IceSDK::Systems;
using namespace IceSDK::Components;
using namespace IceSDK::Graphics;
using namespace IceSDK::Graphics::Components;

void SpriteRenderingSystem::Tick(float pDelta) {
    ICESDK_PROFILE_FUNCTION();
}

void SpriteRenderingSystem::Draw(float pDelta) {
    ICESDK_PROFILE_FUNCTION();

    const auto registry = this->_registry.lock();
    if (registry == nullptr)
        return;

    auto spriteGroup = registry->view<SpriteComponent>();
    for (auto rawSpriteEntity : spriteGroup) {
        auto spriteEntity = Entity(this->_registry, rawSpriteEntity);

        auto &transform = spriteEntity.GetComponent<TransformComponent>();
        auto &sprite = spriteEntity.GetComponent<SpriteComponent>();

        if (sprite.texture == nullptr || !bgfx::isValid(sprite.texture->GetHandle()))
            continue;

        if (spriteEntity.HasComponent<TileComponent>()) {
            auto &tile = spriteEntity.GetComponent<TileComponent>();
            if (spriteEntity.HasComponent<AnimationComponent>()) {
                auto &anim = spriteEntity.GetComponent<AnimationComponent>();
                anim.last_frame += pDelta;
                if (anim.last_frame >= anim.delay) {
                    // step
                    anim.last_frame = 0;
                    anim.current_frame++;
                    if (anim.current_frame >= anim.frames.size()) {
                        anim.current_frame = 0;
                    }
                    tile.info.x = anim.frames[anim.current_frame].x;
                    tile.info.y = anim.frames[anim.current_frame].y;
                }
            }
            GetGameBase()->GetSpriteBatch()->SubmitTiledSprite(sprite.texture, transform.position, sprite.size, tile.info, { 1, 1, 1, 1 });
        } else
            GetGameBase()->GetSpriteBatch()->SubmitTexturedQuad(sprite.texture, transform.position, sprite.size, { 1, 1, 1, 1 });
    }
}