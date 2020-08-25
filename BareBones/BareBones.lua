local CurrentDir = path.getabsolute('.')
local OutDir = path.join(BIN_DIR, "win64_" .. _ACTION, "bin")
local AssetDir = path.join(OutDir, "Assets")

project "BareBones"
    kind "ConsoleApp"

    files { "./**.h*", "./**.c*" }

    includedirs {
        "./include",
        "."
    }

    IceSDK()
    BuildAssets "BareBones"

    debugdir (OutDir)
