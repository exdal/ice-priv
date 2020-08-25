#include "AudioAsset.h"

using namespace IceSDK;
using namespace IceSDK::Assets;

AudioAsset::AudioAsset(std::vector<uint8_t> pData) : _data(std::move(pData))
{
}

Memory::Ptr<IceSDK::Audio::Audio> AudioAsset::LoadAudio() const
{
	return GetGameBase()->GetAudioSystem()->Load(this->_data);
}

eAssetType AudioAsset::GetAssetType()
{
	return eAssetType::Audio;
}

std::vector<uint8_t> AudioAsset::ToByteArray() const
{
	return this->_data;
}

AudioAsset AudioAsset::From(std::string pName, std::vector<uint8_t> pData)
{
	return AudioAsset{pData};
}
