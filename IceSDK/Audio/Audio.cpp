#include "Audio.h"

#include <fmod_errors.h>
#include <iostream>

#include "AudioSystem.h"
#include "fmod.hpp"

using namespace IceSDK::Audio;

Audio::~Audio()
{
	auto *channel = static_cast<FMOD::Channel *>(this->_sound_channel);
	if (channel != nullptr)
	{
		channel->stop();
		this->_sound_channel = nullptr;
	}
}

void Audio::Play()
{
	this->_frequency = 0.0;

	if (this->_sound_channel != nullptr)
		this->Stop(); // Stop before we play... prevents a memory leak

	const auto result = g_FMODSystem->playSound(this->_sound, nullptr, false, &this->_sound_channel);
	if (result != FMOD_OK)
		std::cout << "FMOD: " << FMOD_ErrorString(result) << std::endl;
}

void Audio::Pause(const bool pIsPaused) const
{
	if (this->_sound_channel == nullptr)
		return;

	this->_sound_channel->setPaused(pIsPaused);
}

void Audio::Stop()
{
	if (this->_sound_channel == nullptr)
		return;

	this->_sound_channel->stop();

	this->_sound_channel = nullptr;
}

void Audio::SetVolume(const float pVolume) const
{
	if (this->_sound_channel == nullptr)
		return;

	this->_sound_channel->setVolume(pVolume);
}

void Audio::SetFrequencyMultiplier(const float pMultiplier)
{
	if (this->_sound_channel == nullptr)
		return;

	if (this->_frequency == 0.0f && pMultiplier != 1.0f)
		this->_sound_channel->getFrequency(&this->_frequency);
	else
		this->_frequency = 0.0f;

	this->_sound_channel->setFrequency(this->_frequency * pMultiplier);
}

void Audio::Seek(const uint32_t pPosition) const
{
	if (this->_sound_channel == nullptr)
		return;

	this->_sound_channel->setPosition(pPosition, FMOD_TIMEUNIT_MS);
}

void Audio::Reset()
{
	this->Stop();
	this->_frequency = 0.0;
}

uint32_t Audio::Length() const
{
	if (this->_sound == nullptr)
		return 0;

	uint32_t length;
	this->_sound->getLength(&length, FMOD_TIMEUNIT_MS);

	return length;
}
