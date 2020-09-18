#include "GameBase.h"
#include "imgui.h"

#include "Graphics/Fonts/FontFace.h"
#include "Graphics/EntityHelper.h"
#include "Graphics/ImGui/Widgets/SceneGraph.h"

#include "Utils/Logger.h"

using namespace IceSDK;

class Game final : public GameBase
{
public:
protected:
	void Init() override
	{
		this->_active_scene = std::make_shared<Scene>();
	}

	void InitDraw() override
	{
		Graphics::Entity::Init(this->GetShaderManager());
		Graphics::Entity::InitScene(this->_active_scene);

		_texture = this->GetAssetManager()->LoadTexture("/Assets/Mempler.png");
		_sprite = Graphics::Entity::CreateSprite(this->_active_scene, this->GetShaderManager(), _texture);

		Graphics::FontFace::Init();

		_face = Graphics::FontFace::FromFile("./RobotoMono-Regular.ttf", 24);
		if (_face == nullptr)
			ICESDK_CRITICAL("Failed to initialize FontFace!");

		_font_atlas = _face->GetAtlas(0);
	}

	void Draw(float pDelta) override
	{
		// ImGuiWidgets::AssetBrowser::Frame(this->GetAssetManager());
		ImGuiWidgets::SceneGraph::Frame(this->GetActiveScene());

		ImGui::Begin("Font Atlas");
		{
			if (_font_atlas != nullptr)
				ImGui::Image((ImTextureID)this->_font_atlas->GetHandle().idx, {FT_ATLAS_SIZE, FT_ATLAS_SIZE});
		}
		ImGui::End();
	}

private:
	Memory::Ptr<Graphics::Texture2D> _texture;
	Memory::Ptr<Graphics::Texture2D> _font_atlas;
	Entity _sprite;
	Memory::Ptr<Graphics::FontFace> _face;
};

Memory::Ptr<Game> g_Game;
Memory::Ptr<IceSDK::GameBase> g_GameBase;

int IceSDKMain()
{
	g_Game = std::make_shared<Game>();
	g_GameBase = g_Game;

	g_Game->Run();

	g_Game = nullptr;
	g_GameBase = nullptr;
	return 0;
}

Memory::Ptr<GameBase> GetGameBase()
{
	return g_GameBase;
}
