#pragma once

#include "Utils/Memory.h"

#include "Graphics/Texture2D.h"

#include <bgfx/bgfx.h>
#include <glm/glm.hpp>

#include <array>

#define QUAD_COUNT 4

namespace IceSDK::Graphics {
    static const uint32_t _maxQuads = 20000;
    static const uint32_t _maxVertices = _maxQuads * 4;
    static const uint32_t _maxIndices = _maxQuads * 6;
    static const uint32_t _maxTextureSlots = 16;

    struct VertexInfo {
        glm::vec2 pos;
        glm::vec2 texture_pos;
        glm::vec4 color;
        glm::vec2 batch_info;
    };

    struct BatchStats {
        uint32_t drawCalls;
        uint32_t quadCount;
    };

    class SpriteBatch {
    public:
        SpriteBatch();
        ~SpriteBatch();

        void NewFrame();
        void EndFrame();
        void Flush();
        void FlushReset();

        void SubmitTexturedQuad(Memory::Ptr<Texture2D> texture, const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);
        void SubmitTexturedQuad(Memory::Ptr<Texture2D> texture, const glm::mat4 &transform, const glm::vec4 &color);

        void SubmitTiledSprite(Memory::Ptr<Texture2D> texture, const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &tileInfo, const glm::vec4 &color);
        void SubmitTiledSprite(Memory::Ptr<Texture2D> texture, const glm::mat4 &transform, const glm::vec4 &tileInfo, const glm::vec4 &color);

        void CheckIndexes();
        void DrawIndexed(const glm::mat4 &transform, glm::vec4 vertexpos[QUAD_COUNT], const std::array<glm::vec2, QUAD_COUNT> &uvs, const glm::vec4 &color,
            float textureID = 0.f, uint32_t indexCount = 6);

        float SetTexture(Memory::Ptr<Texture2D> texture);

        std::array<glm::vec2, QUAD_COUNT> MakeTiled(Memory::Ptr<Texture2D> texture, const glm::vec4 &tileInfo);

        BatchStats &GetBatchStats();
        void ResetStats();

    private:
        /*
            Sprite batch,
            some raw pointers don't require smart pointers, let
            them stay raw. we already delete them at the end so
            don't bother adding any smart pointer for quads or
            vertex_XX
        */
        BatchStats m_stats;
        uint32_t m_indexes;
        uint32_t m_textureIndex;

        glm::vec4 m_vertexPositions[4];
        std::array<Memory::Ptr<Texture2D>, _maxTextureSlots> m_textureSlots;
        //                                 ^^^^^^^^^^^^^^^^ texture count - we can get it with bgfx::caps
        std::array<bgfx::UniformHandle, _maxTextureSlots> m_textureUniforms; // size of that member must be same as _textureSlots

        VertexInfo *m_vertexBuffer;
        VertexInfo *m_vertexBufferPtr;

        bgfx::VertexLayout m_vertexLayout;
        bgfx::IndexBufferHandle m_indexHandle;
    };

} // namespace IceSDK::Graphics
