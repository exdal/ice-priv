#include "pch.h"

#include "Audio/AudioSystem.h"

#include "Utils/FileSystem.h"
#include "Utils/Instrumentor.h"
#include "Utils/Logger.h"

using namespace IceSDK::Audio;

#if defined(ICESDK_FMOD)
FMOD::System *g_FMODSystem = nullptr;
#endif

AudioSystem::AudioSystem() {
#ifdef ICESDK_FMOD
    assert(g_FMODSystem && "AudioSystem::Init() wasn't called or failed! Could not "
                           "continue...");
#else
#warning "No AudioSystem!"
#endif
}

void AudioSystem::Init() {
    ICESDK_PROFILE_FUNCTION();

#if defined(ICESDK_FMOD)
    if (FMOD::Debug_Initialize(FMOD_DEBUG_LEVEL_LOG))
        ICESDK_ERROR("Failed to initialize FMOD::Debug");

    if (FMOD::System_Create(&g_FMODSystem) != FMOD_OK)
        ICESDK_CRITICAL("Failed to create FMOD::System!");

    if (g_FMODSystem->init(2048, FMOD_INIT_NORMAL, nullptr) != FMOD_OK)
        ICESDK_CRITICAL("Failed to initialize FMOD::System!");
#endif
}

IceSDK::Memory::Ptr<Sound> AudioSystem::Load(std::string_view _path) {
    return nullptr;
}

IceSDK::Memory::Ptr<Sound> AudioSystem::Load(uint8_t *_data, uint32_t _dataSize) {
    ICESDK_PROFILE_FUNCTION();

#if defined(ICESDK_FMOD)
    FMOD_CREATESOUNDEXINFO sound_info{};

    auto sound = Memory::Ptr<Sound>(new Sound);

    sound_info.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    sound_info.length = _dataSize;

    const auto result = g_FMODSystem->createSound((const char *)_data, FMOD_OPENMEMORY, &sound_info, &sound->_sound);
    if (result != FMOD_OK) {
        ICESDK_ERROR("FMOD: %s", FMOD_ErrorString(result));
        return nullptr;
    }

    this->_loaded_audio.push_back(sound);

    return sound;
#endif
}
