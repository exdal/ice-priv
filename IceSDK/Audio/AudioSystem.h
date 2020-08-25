#pragma once

#include <string>

#include "Audio.h"
#include "AudioEnums.h"
#include "Utils/Memory.h"

#include <vector>

extern FMOD::System *g_FMODSystem;

namespace IceSDK
{
	namespace Audio
	{
		class AudioSystem
		{
		public:
			explicit AudioSystem();

			static void Init();

			Memory::Ptr<Audio> Load(const std::string &pPath);
			Memory::Ptr<Audio> Load(std::vector<uint8_t> pBuffer);

			void CreateGroup(const std::string &pName);

		private:
			std::vector<Memory::Ptr<Audio>> _loaded_audio;
			eAudioEngine _audio_engine = FMOD;

			friend Audio;
		};
	} // namespace Audio
} // namespace IceSDK
