#include "AssetBrowser.h"

#include "imgui.h"

using namespace IceSDK;
using namespace IceSDK::ImGuiWidgets;

void ShowToolTip(Assets::AssetFile *assetFile, Assets::AssetRegion *entry)
{
    if (!ImGui::IsItemHovered())
        return;

    ImGui::BeginTooltip();
    {
        auto textWidth = 0.0f;

        textWidth = ImGui::CalcTextSize(entry->name).x;
        ImGui::Text("Path:");
        ImGui::SameLine(400.0f - textWidth);
        ImGui::Text(entry->name);

        auto sizeInText = Memory::SizeInText(entry->size);
        textWidth = ImGui::CalcTextSize(sizeInText.c_str()).x;
        ImGui::Text("Size In Memory:");
        ImGui::SameLine(400.0f - textWidth);
        ImGui::Text(sizeInText.c_str());

        auto typeString = "";
        switch (entry->type)
        {
        case Assets::eAssetType::Texture:
            typeString = "Texture";
            break;
        case Assets::eAssetType::AnimatedTexture:
            typeString = "AnimatedTexture";
            break;
        case Assets::eAssetType::Audio:
            typeString = "Audio";
            break;
        case Assets::eAssetType::Video:
            typeString = "Video";
            break;
        case Assets::eAssetType::Particle:
            typeString = "Particle";
            break;
        case Assets::eAssetType::Shader:
            typeString = "Particle";
            break;

        case Assets::eAssetType::Text:
            typeString = "Text";
            break;
        default:
            typeString = "Unknown";
            break;
        }

        textWidth = ImGui::CalcTextSize(typeString).x;
        ImGui::Text("Type:");
        ImGui::SameLine(400.0f - textWidth);
        ImGui::Text(typeString);
    }
    ImGui::EndTooltip();
}

void AssetBrowser::Init(Memory::Ptr<Assets::AssetManager> manager)
{
}

void AssetBrowser::Frame(Memory::Ptr<Assets::AssetManager> manager)
{
    ImGui::SetNextWindowSize(ImVec2(750, 300));
    ImGui::Begin("Asset Browser");
    {
        // Left view of the asset browser
        // the "File tree"
        ImGui::SetNextWindowContentSize(ImVec2(200, -1));
        ImGui::BeginChild("AssetBrowser::FileTree");
        {
            auto i = 0;
            auto off = 0.0f;
            for (auto &assetFile : manager->GetAll())
            {
                ImGui::Text("%i:", i);

                off += 10.0f;

                for (auto &assetEntry : assetFile->header.asset_register)
                {
                    auto cursorPosX = ImGui::GetCursorPosX();
                    ImGui::SetCursorPosX(cursorPosX + off);

                    ImGui::Text(assetEntry.name);

                    ShowToolTip(assetFile.get(), &assetEntry);
                }
            }
        }
        ImGui::EndChild();
    }
    ImGui::End();
}
