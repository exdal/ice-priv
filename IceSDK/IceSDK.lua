function IceSDKLibs()
    FMOD()
    GLFW()
    STB()
    IMGUI()
    ICONFONTCPPHEADERS()

    -- BGFX
        includedirs {
            path.join(BX_DIR, "include"),
            path.join(BGFX_DIR, "include"),
            path.join(BIMG_DIR, "include"),
        }

        links {
            "bgfx",
            "bimg",
            "bimg_decode",
            "bx",
        }

        configuration { "mingw*" }
            targetextension ".exe"
            links {
                "gdi32",
                "psapi",
            }

        configuration { "vs20*", "x32 or x64" }
            links {
                "gdi32",
                "psapi",
            }

        configuration { "Linux" }
            links {
                "GL"
            }

        configuration { }
end

function IceSDKDefines()
    configuration { "Windows" }
        defines { "ICESDK_WIN32" }

    configuration { "Linux" }
        defines { "ICESDK_LINUX" }

    configuration { "Android" }
        defines { "ICESDK_ANDROID" }

    configuration { "Debug" }
        defines { "ICESDK_DEBUG" }

    configuration { "Release" }
        defines { "ICESDK_RELEASE" }
        
    configuration { }
end

project "IceSDK"
    kind "StaticLib"

    files {
        "**.h*", "**.c*"
    }

    includedirs {
        ".",
    }

    IceSDKLibs()
    IceSDKDefines()

function IceSDK()
    includedirs {
        path.join(MODULE_DIR, "IceSDK")
    }

    links {
        "IceSDK"
    }

    IceSDKDefines()
    IceSDKLibs()
end
