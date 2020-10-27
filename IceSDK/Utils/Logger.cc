#include "pch.h"

#include "Utils/Logger.h"

// https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Core/Log.h
// Apache2 license

using namespace IceSDK;

static bool g_LoggerInitialized = false;

void Log::Init() {
    

    g_LoggerInitialized = true;
}
