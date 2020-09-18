#pragma once

#include "Graphics/Texture2D.h"
#include "Graphics/Shaders/ShaderManager.h"

#include "ECS/Scene.h"
#include "ECS/Entity.h"

namespace IceSDK::Graphics::Entity
{
	void Init(const Memory::Ptr<Graphics::Shaders::ShaderManager> &pShaderManager);
	void InitScene(const Memory::Ptr<IceSDK::Scene> &pScene);

	IceSDK::Entity CreateSprite(
		Memory::Ptr<IceSDK::Scene> pScene,
		Memory::Ptr<Shaders::ShaderManager> pShaderManager,
		Memory::Ptr<Texture2D> pTex);
} // namespace IceSDK::Graphics::Entity
