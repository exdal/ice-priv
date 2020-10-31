#include "Assets/TextAsset.h"

#include "Utils/FileSystem.h"
#include "Utils/Instrumentor.h"
#include "Utils/Logger.h"

#include "GameBase.h"
#include "Graphics/EntityHelper.h"
#include "Graphics/SpriteBatch/SpriteBatch.h"
#include "Graphics/TextureAtlas/Packer.h"
#include "Graphics/TextureAtlas/TextureAtlas.h"
#include "Graphics/Tilesheet/Tilesheet.h"

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

        _texture = GetAssetManager()->LoadTexture("/Assets/tiles.png", false);

        Graphics::Components::AnimationComponent ac;

        ac.frames.push_back({ 16 * 0, 0 });
        ac.frames.push_back({ 16 * 1, 0 });
        ac.delay = 0.7;

        for (size_t x = 0; x < 20; x++) {
            for (size_t y = 0; y < 15; y++) {
                et = Graphics::Entity::CreateSprite(activeScene, this->GetShaderManager(), _texture, { x * 64, y * 64, 0.f }, { 64, 64 }, { 16 * 0, 0, 16, 16 });
                Graphics::Entity::AttachAnimation(et, ac);
            }
        }
    }

    void Draw(float pDelta) override {
    }

    void Update(float pDelta) override {
    }

private:
    Entity et;
    Memory::Ptr<Graphics::Texture2D> _texture;
};

Memory::Ptr<Game> g_Game;
Memory::Ptr<IceSDK::GameBase> g_GameBase;

#ifdef ICESDK_ANDROID
#include <SDL_main.h>
extern "C" SDLMAIN_DECLSPEC __attribute__((visibility("default"))) int SDL_main(int argc, char *argv[]) {
#else
int IceSDKMain() {
#endif
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
