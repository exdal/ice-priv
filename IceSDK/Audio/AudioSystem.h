#pragma once

#include "Audio/AudioEnums.h"
#include "Audio/Sound.h"

#include "Utils/Memory.h"

#include <string>
#include <vector>

#ifdef ICESDK_FMOD
extern FMOD::System* g_FMODSystem;
#endif

namespace IceSDK
{
    namespace Audio
    {
        class AudioSystem
        {
        public:
            explicit AudioSystem();

            static void Init();

            Memory::Ptr<Sound> Load(std::string_view _path);
            Memory::Ptr<Sound> Load(uint8_t *_data, uint32_t _dataSize);

            void CreateGroup(std::string_view _name);

        private:
            std::vector<Memory::Ptr<Sound>> _loaded_audio;
            eAudioEngine _audio_engine = FMOD;

            friend Sound;
        };
    }  // namespace Audio
}  // namespace IceSDK
