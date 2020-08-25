project "AssetCompiler"
    kind "ConsoleApp"

    files { "./**.h*", "./**.c*" }

    includedirs {
        "./include",
        "."
    }

    IceSDK()
