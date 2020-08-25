#include "AudioSystem.h"
#include "Utils/FileSystem.h"

#include <cassert>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <iostream>

using namespace IceSDK::Audio;

FMOD::System *g_FMODSystem = nullptr;

AudioSystem::AudioSystem()
{
	assert(g_FMODSystem && "AudioSystem::Init() wasn't called or failed! Could not continue...");
}

void AudioSystem::Init()
{
	if (FMOD::Debug_Initialize(FMOD_DEBUG_LEVEL_LOG))
		std::cout << "Failed to initialize FMOD::Debug" << std::endl;

	if (FMOD::System_Create(&g_FMODSystem) != FMOD_OK)
		std::cout << "Failed to create FMOD::System!" << std::endl; // TODO: Fatal crash!

	if (g_FMODSystem->init(2048, FMOD_INIT_NORMAL, nullptr) != FMOD_OK)
		std::cout << "Failed to initialize FMOD::System!" << std::endl; // TODO: Fatal crash!
}

IceSDK::Memory::Ptr<Audio> AudioSystem::Load(const std::string &pPath)
{
	return this->Load(FileSystem::ReadBinaryFile(pPath));
}

IceSDK::Memory::Ptr<Audio> AudioSystem::Load(std::vector<uint8_t> pBuffer)
{
	FMOD_CREATESOUNDEXINFO sound_info{};

	auto audio = Memory::Ptr<Audio>(new Audio);

	sound_info.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
	sound_info.length = static_cast<uint32_t>(pBuffer.size());

	const auto result = g_FMODSystem->createSound(reinterpret_cast<const char *>(pBuffer.data()),
												  FMOD_OPENMEMORY,
												  &sound_info,
												  &audio->_sound);
	if (result != FMOD_OK)
	{
		std::cout << "FMOD: " << FMOD_ErrorString(result) << std::endl;
		return nullptr;
	}

	this->_loaded_audio.push_back(audio);

	return audio;
}
