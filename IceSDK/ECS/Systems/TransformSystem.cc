#include "pch.h"

#include "ECS/Systems/TransformSystem.h"

#include "ECS/Components/TransformComponent.h"
#include "ECS/Entity.h"

using namespace IceSDK;
using namespace IceSDK::Systems;
using namespace IceSDK::Components;

void TransformSystem::Draw(float pDelta) {
}

void TransformSystem::Tick(float pDelta) {
    const auto registry = this->_registry.lock();
    if (registry == nullptr)
        return;

    auto transformGroup = registry->view<TransformComponent>();

    for (auto rawTransformEntity : transformGroup) {
        auto spriteEntity = Entity(this->_registry, rawTransformEntity);
        auto &transform = spriteEntity.GetComponent<TransformComponent>();

        transform.model_matrix = glm::translate(glm::mat4(1.0f), { transform.position.x, transform.position.y, 1 }) *
                                 glm::scale(glm::mat4(1.0f), { transform.scale.x, transform.scale.y, 1.0f });
    }
}
