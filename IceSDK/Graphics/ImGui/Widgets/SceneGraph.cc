#include "pch.h"

#include "Graphics/ImGui/Widgets/SceneGraph.h"

#include "ECS/Components/BaseComponent.h"
#include "ECS/Components/TransformComponent.h"

#include "GameBase.h"

#include <imgui.h>

using namespace IceSDK;
using namespace IceSDK::ImGuiWidgets;

static bool init = false;

void SceneGraph::Frame(const Memory::WeakPtr<Scene> pScene) {
    ImGui::Begin("Scene Graph");
    if (!init) {
        ImGui::SetWindowSize(ImVec2(200, 400));
        ImGui::SetWindowPos(ImVec2(10, 10));
        init = true;
    }
    {
        ImGui::Text("FPS: %d", GetGameBase()->FPS);
        ImGui::Text("Frame Delay: %d", GetGameBase()->FrameMS);
        ImGui::Text("Draw Calls: %d", GetGameBase()->GetSpriteBatch()->GetBatchStats().drawCalls);
        ImGui::Text("Quads: %d(V: %d, I: %d)", GetGameBase()->GetSpriteBatch()->GetBatchStats().quadCount, GetGameBase()->GetSpriteBatch()->GetBatchStats().quadCount * 4,
            GetGameBase()->GetSpriteBatch()->GetBatchStats().quadCount * 6);
            
        GetGameBase()->GetSpriteBatch()->ResetStats();
        auto registry = pScene.lock()->GetRegistry().lock();
    }
    ImGui::End();
}
