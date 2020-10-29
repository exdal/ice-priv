#include "pch.h"
#include "GameBase.h"
#include "Utils/Instrumentor.h"
#include "Utils/Logger.h"

using namespace IceSDK;

GameBase::GameBase() {
    ICESDK_PROFILE_BEGIN_SESSION("Startup", "Benchmark-Startup.json");

    ICESDK_INFO("Powered by IceSDK V0.0.0b");

#if ICESDK_FMOD
    ICESDK_INFO("AudioSystem: FMOD");
#else
    ICESDK_INFO("AudioSystem: NONE");

#endif

    ICESDK_INFO("GAL        : BGFX");

    Audio::AudioSystem::Init();

    this->_window = std::make_shared<Graphics::GameWindow>(1280, 800, "IceSDK: Game Window");
    this->_audio_system = std::make_shared<Audio::AudioSystem>();
    this->_sprite_batch = std::make_shared<Graphics::SpriteBatch>();
    this->_asset_manager = std::make_shared<Assets::AssetManager>();
    this->_shader_manager = std::make_shared<Graphics::Shaders::ShaderManager>();
    this->_input_pipeline = std::make_shared<Input::InputPipeline>();

    this->_asset_manager->Init();

    ICESDK_PROFILE_END_SESSION();
}

GameBase::~GameBase() {
    this->_active_scene = nullptr;
    this->_asset_manager = nullptr;
    this->_sprite_batch = nullptr;
    this->_audio_system = nullptr;
    this->_shader_manager = nullptr;
    this->_input_pipeline = nullptr;
    this->_window = nullptr;
}

void GameBase::Run() {
    ICESDK_PROFILE_BEGIN_SESSION("Runtime", "Benchmark-Runtime.json");

    this->Init();

    this->_input_pipeline->Init();
    this->_window->SetDrawCallback(GameBase::InternalDraw);
    this->_window->SetDrawInitCallback(GameBase::InternalDrawInit);
    while (!this->_exit) {
        ICESDK_PROFILE_SCOPE("GameBase::MainLoop");

        this->_window->Update();

        // Calculate delta time
        const auto now = bx::getHPCounter();
        const auto frameTime = now - this->_last_delta;
        this->_last_delta = now;

        const auto freq = static_cast<float>(bx::getHPFrequency());
        const auto delta = static_cast<float>(frameTime) / freq;

        GameBase::InternalTick(delta);
        if (this->_window->ShouldClose())
            break;
    }

    ICESDK_PROFILE_END_SESSION();

    ICESDK_PROFILE_BEGIN_SESSION("Shutdown", "Benchmark-Shutdown.json");
    GameBase::InternalShutdown();
    ICESDK_PROFILE_END_SESSION();
}

Memory::Ptr<Audio::AudioSystem> GameBase::GetAudioSystem() const {
    return this->_audio_system;
}

Memory::Ptr<Graphics::SpriteBatch> GameBase::GetSpriteBatch() const {
    return this->_sprite_batch;
}

Memory::Ptr<Assets::AssetManager> GameBase::GetAssetManager() const {
    return this->_asset_manager;
}

Memory::Ptr<Graphics::GameWindow> GameBase::GetGameWindow() const {
    return this->_window;
}

Memory::Ptr<Graphics::Shaders::ShaderManager> GameBase::GetShaderManager() const {
    return this->_shader_manager;
}

Memory::Ptr<Input::InputPipeline> GameBase::GetInputPipeline() const {
    return this->_input_pipeline;
}

Memory::WeakPtr<Scene> GameBase::GetActiveScene() const {
    return this->_active_scene;
}

// User implemented functions
void GameBase::Init() {
}

void GameBase::Shutdown() {
}

void GameBase::Update(float pDelta) {
    BX_UNUSED(pDelta)
}

void GameBase::Draw(float pDelta) {
    BX_UNUSED(pDelta)
}

void GameBase::InitDraw() {
}

// Internal functions
void GameBase::InternalDraw(const float pDelta) {
    ICESDK_PROFILE_FUNCTION();

    auto game = GetGameBase();

    // Begin Scene
    if (game->_active_scene != nullptr)
        game->_active_scene->Draw(pDelta);
// End Scene

// Begin ImGui
#if ICESDK_USE_IMGUI
#ifdef ICESDK_GLFW
    ImGui_ImplGlfw_NewFrame();
#elif defined(ICESDK_SDL2)
    ImGui_ImplSDL2_NewFrame(game->_window->_window);
#else
#warning "Undefined Graphics API"
#endif
    ImGui::NewFrame();
    game->Draw(pDelta);
    imguiEndFrame();
#endif
}

void GameBase::InternalTick(const float pDelta) {
    ICESDK_PROFILE_FUNCTION();

    auto game = GetGameBase();

    // Begin Scene
    if (game->_active_scene != nullptr)
        game->_active_scene->Tick(pDelta);
    // End Scene

    game->Update(pDelta);
}

void GameBase::InternalDrawInit() {
    ICESDK_PROFILE_FUNCTION();

    auto game = GetGameBase();
    game->InitDraw();
}

void GameBase::InternalShutdown() {
    ICESDK_PROFILE_FUNCTION();

    auto game = GetGameBase();
    game->Shutdown();
}

static Memory::Ptr<bx::AllocatorI> g_Allocator = std::make_shared<bx::DefaultAllocator>();

Memory::Ptr<bx::AllocatorI> GetAllocator() {
    return g_Allocator;
}

Memory::Ptr<Graphics::GameWindow> GetWindow() {
    return GetGameBase()->GetGameWindow();
}

IceSDK::Memory::Ptr<IceSDK::Audio::AudioSystem> GetAudioSystem() {
    return GetGameBase()->GetAudioSystem();
}

#if defined(ICESDK_SDL2) && defined(ICESDK_ANDROID)
int IceSDKMain();
extern "C" SDLMAIN_DECLSPEC __attribute__((visibility("default"))) int SDL_main(int argc, char *argv[]) {
    return IceSDKMain();
}
#endif
