#include "Utils/FileSystem.h"
#include "Utils/Instrumentor.h"
#include "Utils/Logger.h"

#include "GameBase.h"
#include "Graphics/Debug/Draw.h"
#include "Graphics/EntityHelper.h"
#include "Graphics/ImGui/Widgets/SceneGraph.h"
#include "Graphics/SpriteBatch/SpriteBatch.h"
#include "Graphics/TextureAtlas/Packer.h"
#include "Graphics/TextureAtlas/TextureAtlas.h"
#include "Graphics/Tilesheet/Tilesheet.h"

#include <imgui.h>

using namespace IceSDK;

class Game final : public GameBase {
public:
protected:
    void Init() override {
        /* TODO: create a function in GameBase */
        this->_active_scene = std::make_shared<Scene>();
    }

    void InitDraw() override {
        auto activeSceneLock = this->GetActiveScene();
        auto activeScene = activeSceneLock.lock();

        // TODO: let GameBase initialize this.
        Graphics::Entity::Init(this->GetShaderManager());
        Graphics::Entity::InitScene(activeScene);

        atlas = new Graphics::TextureAtlas(1024, false);

        atlas->Push("/Assets/mario.png");
        atlas->Push("/Assets/windows_64x64.png");
        atlas->Push("/Assets/Ground.png");
        atlas->Push("/Assets/Box.png");
        std::string asd = GetAssetManager()->LoadText("/Assets/hello.txt");

        Graphics::Entity::CreateSprite(activeScene, this->GetShaderManager(), atlas->Texture(), { 10.f, 10.f, 0.f }, atlas->SizeOf(1), atlas->CoordinatesOf(1));
    }

    void Draw(float pDelta) override {
        ImGuiWidgets::SceneGraph::Frame(this->GetActiveScene());
    }

    /* Not really needed as we use Systems by default */
    void Update(float pDelta) override {
    }

private:
    Graphics::TextureAtlas *atlas;
    /* Make sure the code above is uncommented.
    Entity _text;
    IceSDK::Graphics::FontFaceHandle _faceHandle;
    */
};

Memory::Ptr<Game> g_Game;
Memory::Ptr<IceSDK::GameBase> g_GameBase;

int IceSDKMain {
    g_Game = std::make_shared<Game>();
    g_GameBase = g_Game;

    g_Game->Run();

    g_Game = nullptr;
    g_GameBase = nullptr;
    return 0;
}

Memory::Ptr<GameBase> GetGameBase() {
    return g_GameBase;
}
