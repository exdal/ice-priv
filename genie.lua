solution "IceSDK"
    language "C++"
    configurations { "Debug", "Release" }
    startproject "BareBones"

    platforms {
        "x64",
    }

    startproject "BareBones"

MODULE_DIR = path.getabsolute(".")
BIN_DIR = path.join(MODULE_DIR, "bin")
THIRD_PARTY_DIR = path.join(MODULE_DIR, "third-party")

GLFW_DIR               = path.join(THIRD_PARTY_DIR, 'glfw' )
BX_DIR                 = path.join(THIRD_PARTY_DIR, 'bx'   )
BIMG_DIR               = path.join(THIRD_PARTY_DIR, 'bimg' )
BGFX_DIR               = path.join(THIRD_PARTY_DIR, 'bgfx' )
FMOD_DIR               = path.join(THIRD_PARTY_DIR, 'fmod' )
STB_DIR                = path.join(THIRD_PARTY_DIR, 'stb'  )
IMGUI_DIR              = path.join(THIRD_PARTY_DIR, 'imgui')
ICONFONTCPPHEADERS_DIR = path.join(THIRD_PARTY_DIR, 'iconfontcppheaders')

function copyLib()
end

dofile (path.join(BX_DIR, "scripts/toolchain.lua"))
if not toolchain(BIN_DIR, THIRD_PARTY_DIR) then
	return
end

group "Third-party"
    -- Audio
        -- FMOD
        dofile './third-party/fmod/fmod.lua'

    -- Graphics
        -- GLFW
        dofile './third-party/glfw/glfw.lua'

        -- BGFX
        dofile './third-party/bx/scripts/bx.lua'
        dofile './third-party/bgfx/scripts/bgfx.lua'
        dofile './third-party/bimg/scripts/bimg.lua'
        dofile './third-party/bimg/scripts/bimg_decode.lua'

    -- Utilities
        -- STB
        dofile './third-party/stb/stb.lua'
        -- ImGui
        dofile './third-party/imgui/imgui.lua'
        -- iconfontcppheaders
        dofile './third-party/iconfontcppheaders/iconfontcppheaders.lua'


    BGFX_CONFIG = {
        "BGFX_CONFIG_MULTITHREADED=1"
    }
    bgfxProject("", "StaticLib", BGFX_CONFIG)

function BuildAssets(_proj_name)
    local CurrentDir = path.getabsolute('.')

    local OutDir = ""
    if os.is("Windows") then
        if os.is64bit() then
            OutDir = path.join(BIN_DIR, "win64_" .. _ACTION, "bin")
        else
            OutDir = path.join(BIN_DIR, "win32_" .. _ACTION, "bin")
        end
    end

    if os.is("Linux") then
        if os.is64bit() then
            OutDir = path.join(BIN_DIR, "linux64_gcc", "bin")
        else
            OutDir = path.join(BIN_DIR, "linux32_gcc", "bin")
        end
    end

    local AssetOutDir = path.join(OutDir, './Assets')

    configuration { "Windows" }
        prebuildcommands {
            'powershell ' .. path.join(MODULE_DIR, 'tools/build_assets.bat') .. ' ' .. _proj_name,
        }
        postbuildcommands {
            'mkdir \"' .. AssetOutDir .. "\"",
            'powershell copy "' .. path.join(CurrentDir, "./.build/*.pxl") .. '" "' .. AssetOutDir .. '"',
        }

    configuration { "Linux" }
        prebuildcommands {
            path.join(MODULE_DIR, 'tools/build_assets.sh') .. ' ' .. _proj_name,
        }
        postbuildcommands {
            'mkdir -p \"' .. AssetOutDir .. "\"",
            'cp -r ' .. path.join(CurrentDir, "./.build/*.pxl") .. ' "' .. AssetOutDir .. '"',
        }

    configuration {  }
end

group "Projects"
    -- IceSDK
    dofile './IceSDK/IceSDK.lua'

    -- BareBones
    dofile './BareBones/BareBones.lua'

    group "Projects/Tools"
        -- tools/AssetCompiler
        dofile './tools/AssetCompiler/AssetCompiler.lua'
