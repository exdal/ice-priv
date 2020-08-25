#pragma once

#include <cstdint>
#include <functional>
#include <string>

#include "bx/thread.h"
#include "bx/timer.h"
#include "GLFW/glfw3.h"

namespace IceSDK
{
	class GameBase;

	using DrawCallback_t = std::function<void(float)>;
	using DrawInitCallback_t = std::function<void()>;

	namespace Graphics
	{
		enum eGameWindowFlags
		{
			None,
			Fullscreen = 1 << 1
		};

		class GameWindow
		{
		public:
			explicit GameWindow(int32_t pWidth, int32_t pHeight, const std::string &pTitle, eGameWindowFlags pFlags = eGameWindowFlags::None);
			~GameWindow();

			void Update();

			void SetDrawCallback(DrawCallback_t pCallback);
			void SetDrawInitCallback(DrawInitCallback_t pCallback);

			// true = Window closed
			bool ShouldClose() const;

		private:
			friend GameBase;

			static void ResizeGameWindow(GLFWwindow *pWindow, int pWidth, int pHeight);
			static int32_t RunThread(bx::Thread *pSelf, void *pUserData);

			GLFWwindow *_window = nullptr;

			DrawCallback_t _draw_callback = nullptr;
			DrawInitCallback_t _draw_init_callback = nullptr;
			uint32_t _width{}, _height{};
			bool _initialized = false;

			int64_t _last_time = bx::getHPCounter();
		};
	} // namespace Graphics
} // namespace IceSDK
