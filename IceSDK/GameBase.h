#pragma once

#include "Assets/AssetManager.h"
#include "Audio/AudioSystem.h"
#include "Utils/Memory.h"
#include "Graphics/Window.h"

#include "bx/allocator.h"

namespace IceSDK
{
	class GameBase
	{
	public:
		explicit GameBase();
		virtual ~GameBase() = default;

		void Run();

		Memory::Ptr<Audio::AudioSystem> GetAudioSystem() const;
		Memory::Ptr<Assets::AssetManager> GetAssetManager() const;
		Memory::Ptr<Graphics::GameWindow> GetGameWindow() const;

	protected:
		virtual void Init();
		virtual void Shutdown();

		virtual void Update(float pDelta);
		virtual void Draw(float pDelta);
		virtual void InitDraw();

	private:
		bool _exit{};

		static void InternalDraw(float pDelta);
		static void InternalDrawInit();
		static void InternalShutdown();

		Memory::Ptr<Graphics::GameWindow> _window;
		Memory::Ptr<Audio::AudioSystem> _audio_system;
		Memory::Ptr<Assets::AssetManager> _asset_manager;

		std::weak_ptr<bx::AllocatorI> _bx_alloc;
	};
} // namespace IceSDK

IceSDK::Memory::Ptr<IceSDK::GameBase> GetGameBase();
