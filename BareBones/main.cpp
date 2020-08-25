#include "GameBase.h"

using namespace IceSDK;

class Game final : public GameBase
{
public:
protected:
	void Init() override
	{
		//auto audio = this->GetAssetManager()->LoadAudio("/Assets/Jonth - Collapse [NCS Release].mp3");

		//audio->Play();
		//audio->SetVolume(0.25f);
	}

private:
};

Memory::Ptr<Game> g_Game;
Memory::Ptr<IceSDK::GameBase> g_GameBase;

int main()
{
	g_Game = std::make_shared<Game>();
	g_GameBase = g_Game;

	g_Game->Run();

	return 0;
}

Memory::Ptr<GameBase> GetGameBase()
{
	return g_GameBase;
}
