#pragma once

#include "Utils/Memory.h"
#include "Assets/AssetManager.h"

namespace IceSDK
{

    namespace ImGuiWidgets
    {

        namespace AssetBrowser
        {
            void Init(Memory::Ptr<Assets::AssetManager> manager);
            void Frame(Memory::Ptr<Assets::AssetManager> manager);
        } // namespace AssetBrowser

    } // namespace ImGuiWidgets

} // namespace IceSDK
