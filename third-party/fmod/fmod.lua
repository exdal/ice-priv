function FMOD()
    includedirs {
        path.join(FMOD_DIR, "inc"),
    }

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

    print(OutDir)

    configuration { "Windows", "x32" }
        links {
            path.join(FMOD_DIR, "lib/win32/x86/fmod_vc")
        }

        postbuildcommands { 'powershell copy "' .. path.join(FMOD_DIR, "lib/win32/x86/fmod.dll") .. '" "' .. OutDir .. '"' }

    configuration { "Windows", "x64" }
        links {
            path.join(FMOD_DIR, "lib/win32/x64/fmod_vc")
        }

        postbuildcommands { 'powershell copy "' .. path.join(FMOD_DIR, "lib/win32/x64/fmod.dll") .. '" "' .. OutDir .. '"'}

    configuration { "Linux", "x32" }
        links {
            'fmod'
        }

        linkoptions {
            "-Wl,-rpath,\\$$ORIGIN"
        }

        postbuildcommands { 'cp ' .. path.join(FMOD_DIR, "lib/linux/x86/libfmod.so.*") .. ' "' .. OutDir .. '"' }

    configuration { "Linux", "x64" }
        links {
            'fmod'
        }

        linkoptions {
            "-Wl,-rpath,\\$$ORIGIN"
        }

        postbuildcommands { 'cp ' .. path.join(FMOD_DIR, "lib/linux/x86_64/libfmod.so.*") .. ' "' .. OutDir .. '"'}


    configuration {  }
end
