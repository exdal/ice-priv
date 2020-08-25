project "GLFW"
    kind "StaticLib"

    files { 
		"include/GLFW/glfw3.h",
		"include/GLFW/glfw3native.h",
		"src/glfw_config.h",
		"src/context.c",
		"src/init.c",
		"src/input.c",
		"src/monitor.c",
		"src/vulkan.c",
		"src/window.c"
    }

    includedirs {
        "./include"
    }
    
    configuration { "Debug" }
        targetdir "bin/Debug"
        defines { "DEBUG" }

    configuration { "Release" }
        targetdir "bin/Release"
        defines { "NDEBUG", "RELEASE" }

    configuration { "Windows" }
        files
        {
            "src/win32_init.c",
            "src/win32_joystick.c",
            "src/win32_monitor.c",
            "src/win32_time.c",
            "src/win32_thread.c",
            "src/win32_window.c",
            "src/wgl_context.c",
            "src/egl_context.c",
            "src/osmesa_context.c"
        }
        defines { "_GLFW_WIN32" }

    configuration { "Linux" }
        files
        {
            "src/x11_init.c",
            "src/x11_monitor.c",
            "src/x11_window.c",
            "src/xkb_unicode.c",
            "src/posix_time.c",
            "src/posix_thread.c",
            "src/glx_context.c",
            "src/egl_context.c",
            "src/osmesa_context.c",
            "src/linux_joystick.c"
        }

        defines { "_GLFW_X11" }

        links {
            "pthread"
        }

    configuration { }

function GLFW()
    includedirs {
        path.join(GLFW_DIR, "include"),
    }

    configuration { "Linux" }
        links {
            'X11',
            'pthread'
        }
    
    configuration { }

    links {
        'GLFW'
    }
end
