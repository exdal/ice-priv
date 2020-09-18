// used for unity tests

#include "GameBase.h"
#include "Utils/Memory.h"

static IceSDK::Memory::Ptr<IceSDK::GameBase> g_GameBase;

class TestGame : public IceSDK::GameBase
{
};

IceSDK::Memory::Ptr<IceSDK::GameBase> GetGameBase()
{
    if (g_GameBase == nullptr)
        g_GameBase = std::make_shared<TestGame>();

    return g_GameBase;
}