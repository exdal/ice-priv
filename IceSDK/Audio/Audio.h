#pragma once

#include <cstdint>
#include <fmod.hpp>

namespace IceSDK
{
	namespace Audio
	{
		class AudioSystem;

		class Audio
		{
		public:
			~Audio();

			void Play();
			void Pause(bool pIsPaused = true) const;
			void Stop();
			void SetVolume(float pVolume = 1.0f) const;

			void SetFrequencyMultiplier(float pMultiplier = 1.0f);
			void Seek(uint32_t pPosition) const;
			void Reset();
			uint32_t Length() const;

		private:
			float _frequency;

			explicit Audio() = default;

			FMOD::Sound *_sound = nullptr;
			FMOD::Channel *_sound_channel = nullptr;

			friend AudioSystem;
		};
	} // namespace Audio
} // namespace IceSDK
