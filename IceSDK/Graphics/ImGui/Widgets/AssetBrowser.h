#pragma once

#include "Utils/Memory.h"
#include "Assets/AssetManager.h"

namespace IceSDK::ImGuiWidgets::AssetBrowser
{
	void Init(Memory::Ptr<Assets::AssetManager> pManager);
	void Frame(Memory::Ptr<Assets::AssetManager> pManager);
}
