#include "GameBase.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "bx/timer.h"
#include "ImGui/bgfx_imgui.h"
#include "ImGui/Widgets/AssetBrowser.h"

using namespace IceSDK;

GameBase::GameBase()
{
	Audio::AudioSystem::Init();

	this->_window = std::make_shared<Graphics::GameWindow>(1280, 720, "IceSDK: Game Window");
	this->_audio_system = std::make_shared<Audio::AudioSystem>();
	this->_asset_manager = std::make_shared<Assets::AssetManager>();

	this->_asset_manager->Init();
	this->_bx_alloc = std::weak_ptr<bx::DefaultAllocator>();
}

void GameBase::Run()
{
	this->Init();
	this->_window->SetDrawCallback(GameBase::InternalDraw);
	this->_window->SetDrawInitCallback(GameBase::InternalDrawInit);

	auto last = bx::getHPCounter();
	while (!this->_exit)
	{
		this->_window->Update();

		// Calculate delta time
		const auto now = bx::getHPCounter();
		const auto frameTime = now - last;
		last = now;

		const auto freq = static_cast<float>(bx::getHPFrequency());
		const auto delta = static_cast<float>(frameTime) / freq;

		this->Update(delta);

		if (this->_window->ShouldClose())
			break;
	}

	GameBase::InternalShutdown();
}

Memory::Ptr<Audio::AudioSystem> GameBase::GetAudioSystem() const
{
	return this->_audio_system;
}

Memory::Ptr<Assets::AssetManager> GameBase::GetAssetManager() const
{
	return this->_asset_manager;
}

Memory::Ptr<Graphics::GameWindow> GameBase::GetGameWindow() const
{
	return this->_window;
}

// User implemented functions
void GameBase::Init()
{
}

void GameBase::Shutdown()
{
}

void GameBase::Update(float pDelta)
{
}

void GameBase::Draw(float pDelta)
{
}

void GameBase::InitDraw()
{
}

// Internal functions
void GameBase::InternalDraw(const float pDelta)
{
	auto game = GetGameBase();

	// Begin ImGui
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	{
		ImGuiWidgets::AssetBrowser::Frame(game->GetAssetManager());
		ImGui::ShowDemoWindow();
	}
	imguiEndFrame();
	// End ImGui

	game->Draw(pDelta);
}

void GameBase::InternalDrawInit()
{
	auto game = GetGameBase();

	imguiCreate(16.0f, nullptr);

	auto &io = ImGui::GetIO();
	ImGui::StyleColorsDark();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	  // Enable Docking

	ImGui_ImplGlfw_InitForBGFX(game->_window->_window, true);

	game->InitDraw();
}

void GameBase::InternalShutdown()
{
	auto game = GetGameBase();

	imguiDestroy();

	game->Shutdown();
}
