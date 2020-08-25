project "ImGui"
    kind "StaticLib"

    files { 
        "*.h*",
        "*.c*",
    }

    includedirs {
        "."
    }
    
    GLFW()

    configuration { "Debug" }
        targetdir "bin/Debug"
        defines { "DEBUG" }

    configuration { "Release" }
        targetdir "bin/Release"
        defines { "NDEBUG", "RELEASE" }

    configuration { }

function IMGUI()
    includedirs {
        path.join(IMGUI_DIR, "."),
        path.join(IMGUI_DIR, "./examples"),
    }

    links {
        'ImGui'
    }

    GLFW()
end
