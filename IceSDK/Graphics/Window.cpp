#include "Window.h"

#include <iostream>
#include <string>

#ifdef ICESDK_WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#elif ICESDK_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#endif
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"

using namespace IceSDK::Graphics;

void GameWindow::ResizeGameWindow(GLFWwindow *pWindow, const int pWidth, const int pHeight)
{
	auto *self = static_cast<GameWindow *>(glfwGetWindowUserPointer(pWindow));

	self->_width = static_cast<uint32_t>(pWidth);
	self->_height = static_cast<uint32_t>(pHeight);

	bgfx::reset(self->_width, self->_height, BGFX_RESET_NONE);
	bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

	self->Update();
}

GameWindow::GameWindow(const int32_t pWidth, const int32_t pHeight, const std::string &pTitle, const eGameWindowFlags pFlags)
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW!" << std::endl; // TODO: fatal crash!
	}

	GLFWmonitor *monitor = nullptr;
	if (pFlags & eGameWindowFlags::Fullscreen)
		monitor = glfwGetPrimaryMonitor();

	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	this->_window = glfwCreateWindow(pWidth, pHeight, pTitle.c_str(), monitor, nullptr);
	if (this->_window == nullptr)
		std::cout << "Failed to create window!" << std::endl; // TODO: fatal crash!

	glfwSetWindowUserPointer(this->_window, this);
	glfwSetWindowSizeCallback(this->_window, &GameWindow::ResizeGameWindow);

	// bgfx::renderFrame();

	bgfx::PlatformData platformData;
#ifdef ICESDK_WIN32
	platformData.nwh = glfwGetWin32Window(this->_window);
#elif ICESDK_LINUX
	platformData.nwh = (void *)glfwGetX11Window(this->_window);
#else
#error "Platform not implemented!"
#endif

	bgfx::Init bgfxInit;
	bgfxInit.type = bgfx::RendererType::Count; // Automatically choose a renderer.
	bgfxInit.platformData = platformData;
	bgfxInit.resolution.width = pWidth;
	bgfxInit.resolution.height = pHeight;
	bgfxInit.resolution.reset = BGFX_RESET_NONE;
	if (!bgfx::init(bgfxInit))
	{
		std::cout << "Failed to initialize BGFX!" << std::endl; // TODO: fatal crash!
	}

	bgfx::setViewClear(0,
					   BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
					   0x443355FF,
					   1.0f,
					   0);
	bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
}

GameWindow::~GameWindow()
{
	bgfx::shutdown();
	glfwDestroyWindow(this->_window);
	glfwTerminate();
}

void GameWindow::Update()
{
	glfwPollEvents();

	const auto now = bx::getHPCounter();
	const auto frameTime = now - _last_time;
	_last_time = now;

	const auto freq = static_cast<float>(bx::getHPFrequency());
	const auto delta = static_cast<float>(frameTime) / freq;

	// This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
	bgfx::touch(0);
	// Use debug font to print information about this example.
	bgfx::dbgTextClear();

	if (this->_draw_init_callback != nullptr && !this->_initialized)
	{
		this->_draw_init_callback();
		this->_initialized = true;
	}
	if (this->_draw_callback != nullptr)
		this->_draw_callback(delta);

	// Advance to next frame. Main thread will be kicked to process submitted rendering primitives.
	bgfx::frame();
}

void GameWindow::SetDrawCallback(const DrawCallback_t pCallback)
{
	this->_draw_callback = pCallback;
}

void GameWindow::SetDrawInitCallback(const DrawInitCallback_t pCallback)
{
	this->_draw_init_callback = pCallback;
}

bool GameWindow::ShouldClose() const
{
	return glfwWindowShouldClose(this->_window);
}
