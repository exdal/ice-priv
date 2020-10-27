#include "pch.h"

#include "Graphics/Systems/TextRenderingSystem.h"

#include "ECS/Entity.h"

#include "Utils/Instrumentor.h"
#include "Utils/Logger.h"
#include "Utils/Math.h"
#include "Utils/String.h"

#include "GameBase.h"
#include "Graphics/Components/SpriteComponent.h"
#include "Graphics/Components/TextComponent.h"
#include "Graphics/Fonts/FontManager.h"

#include <hb-icu.h>

using namespace IceSDK;
using namespace IceSDK::Systems;
using namespace IceSDK::Graphics;
using namespace IceSDK::Graphics::Components;

// https://github.com/tangrams/harfbuzz-example/blob/master/src/hbshaper.h
// under MIT license!
namespace HBFeature {
    const hb_tag_t KernTag = HB_TAG('k', 'e', 'r', 'n'); // kerning operations
    const hb_tag_t LigaTag = HB_TAG('l', 'i', 'g', 'a'); // standard ligature substitution
    const hb_tag_t CligTag = HB_TAG('c', 'l', 'i', 'g'); // contextual ligature substitution
    const hb_tag_t BaseTag = HB_TAG('b', 'a', 's', 'e'); // baseline

    static hb_feature_t LigatureOn = { LigaTag, 1, 0, std::numeric_limits<unsigned int>::max() };
    static hb_feature_t KerningOn = { KernTag, 1, 0, std::numeric_limits<unsigned int>::max() };
    static hb_feature_t CligOn = { CligTag, 1, 0, std::numeric_limits<unsigned int>::max() };
    static hb_feature_t BaseLineOn = { BaseTag, 1, 0, std::numeric_limits<unsigned int>::max() };
} // namespace HBFeature

void TextRenderingSystem::Tick(float pDelta) {
    ICESDK_PROFILE_FUNCTION();
}

void TextRenderingSystem::Draw(float pDelta) {
    ICESDK_PROFILE_FUNCTION();
    const auto registry = this->_registry.lock();
    if (registry == nullptr)
        return;

    auto textGroup = registry->view<TextComponent>();
    for (auto rawTextEntity : textGroup) {
        auto textEntity = Entity(this->_registry, rawTextEntity);

        auto &text = textEntity.GetComponent<TextComponent>();
        auto &sprite = textEntity.GetComponent<SpriteComponent>();

        // Re-render text if it has changed
        if (String::CalculateHash(text.text) + text.font_size != text._old_text) {
            auto fontManager = GetGameBase()->GetFontManager();

            auto [_, fontFace] = fontManager->GetGlyph(text.font_face_handle, text.text[0], text.font_size);

            auto buffer = fontFace->_hb_buffer();
            auto font = fontFace->_hb_font();

            hb_buffer_reset(buffer);

            hb_unicode_funcs_t *icufunctions;
            icufunctions = hb_icu_get_unicode_funcs();
            hb_buffer_set_unicode_funcs(buffer, icufunctions);

            hb_buffer_add_utf8(buffer, text.text.c_str(), text.text.length(), 0, text.text.length());
            hb_buffer_guess_segment_properties(buffer);

            std::vector<hb_feature_t> features;
            features.push_back(HBFeature::KerningOn);
            features.push_back(HBFeature::LigatureOn);
            features.push_back(HBFeature::CligOn);
            features.push_back(HBFeature::BaseLineOn);

            hb_shape(font, buffer, &features[0], features.size());

            unsigned int glyphCount;
            hb_glyph_info_t *glyphInfos = hb_buffer_get_glyph_infos(buffer, &glyphCount);
            hb_glyph_position_t *glyphPositions = hb_buffer_get_glyph_positions(buffer, &glyphCount);

            // Calculate text size
            glm::vec2 textSize{};
            float highestBearing = 0;

            for (size_t i = 0; i < glyphCount; ++i) {
                auto glyphInfo = glyphInfos[i];

                auto [glyph, fontFace] = fontManager->GetGlyph(text.font_face_handle, glyphInfo.codepoint, text.font_size);

                highestBearing = Math::Max(highestBearing, glyph.BearingBase);

                int twidth = pow(2, ceil(log((double)glyph.Size.x) / log(2)));
                int theight = pow(2, ceil(log((double)glyph.Size.y) / log(2)));

                textSize.x += twidth;
                textSize.y = Math::Max(textSize.y, theight);
            }

            auto texture = Texture2D::Create("SpriteText: " + text.text, textSize.x, textSize.y, bgfx::TextureFormat::RGBA8);

            // Render text
            glm::vec2 pen{};
            for (size_t i = 0; i < glyphCount; ++i) {
                auto glyphInfo = glyphInfos[i];
                auto glyphPos = glyphPositions[i];
                glm::vec2 glyphOffset = { glyphPos.x_offset / 64, glyphPos.y_offset / 64 };

                auto [glyph, fontFace] = fontManager->GetGlyph(text.font_face_handle, glyphInfo.codepoint, text.font_size);

                auto baseLine = highestBearing - glyph.GlyphTop;

                auto targetPos = pen - glyphOffset;
                targetPos.y = baseLine;

                texture->Modify({ targetPos, glyph.Size }, glyph.PixelData, bgfx::TextureFormat::RGBA8);

                pen.x += glyphPos.x_advance / 64;
                pen.y += glyphPos.y_advance / 64;
            }

            sprite.size = textSize;
            sprite.texture = texture;

            text._old_text = String::CalculateHash(text.text) + text.font_size;
        }
    }
}
