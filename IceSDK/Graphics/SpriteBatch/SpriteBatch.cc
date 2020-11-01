#include "pch.h"

#include "SpriteBatch.h"

#include "Utils/Logger.h"

#include "GameBase.h"

using namespace IceSDK::Graphics;

SpriteBatch::SpriteBatch() {
    m_vertexBuffer = new VertexInfo[_maxVertices];
    uint32_t *quads = new uint32_t[_maxIndices];
    uint32_t offset = 0;

    for (uint32_t i = 0; i < _maxIndices; i += 6) {
        quads[i + 0] = offset + 0;
        quads[i + 1] = offset + 1;
        quads[i + 2] = offset + 2;

        quads[i + 3] = offset + 2;
        quads[i + 4] = offset + 3;
        quads[i + 5] = offset + 0;

        offset += 4;
    }

    m_indexHandle = bgfx::createIndexBuffer(bgfx::copy(quads, _maxIndices * sizeof(uint32_t)), BGFX_BUFFER_INDEX32);
    delete[] quads;

    m_vertexLayout.begin()
        .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float, true)
        .add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Float)
        .end();

    for (uint32_t i = 0; i < _maxTextureSlots; i++) {
        const std::string uni_name = "s_texColour" + std::to_string(i);
        m_textureUniforms[i] = bgfx::createUniform(uni_name.c_str(), bgfx::UniformType::Sampler);
    }

    m_vertexPositions[0] = { 1.0f, 1.0f, 0, 1 };
    m_vertexPositions[1] = { 1.0f, 0.0f, 0, 1 };
    m_vertexPositions[2] = { 0.0f, 0.0f, 0, 1 };
    m_vertexPositions[3] = { 0.0f, 1.0f, 0, 1 };
}

SpriteBatch::~SpriteBatch() {
}

void SpriteBatch::NewFrame() {
    m_indexes = 0;
    m_vertexBufferPtr = m_vertexBuffer;
    m_textureIndex = 0;
}

void SpriteBatch::EndFrame() {
    Flush();
}

void SpriteBatch::Flush() {
    if (!m_indexes)
        return;
    uint32_t data_size = (uint32_t)((uint8_t *)m_vertexBufferPtr - (uint8_t *)m_vertexBuffer);

    if (bgfx::getAvailTransientVertexBuffer(data_size, m_vertexLayout)) {
        bgfx::TransientVertexBuffer tvb;
        bgfx::allocTransientVertexBuffer(&tvb, data_size, m_vertexLayout);
        memcpy(tvb.data, m_vertexBuffer, data_size);
        bgfx::setVertexBuffer(0, &tvb);
    }

    uint32_t count = m_indexes ? m_indexes : _maxIndices;
    bgfx::setIndexBuffer(m_indexHandle, 0, count);

    for (uint32_t i = 0; i < m_textureIndex; i++) { bgfx::setTexture(i, m_textureUniforms[i], m_textureSlots[i]->GetHandle()); }

    bgfx::setState(0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_ALPHA);

    auto shader = GetGameBase()->GetShaderManager()->LoadProgram("Sprite");
    bgfx::submit(0, shader);
    m_stats.drawCalls++;
}

void SpriteBatch::FlushReset() {
    EndFrame();

    m_indexes = 0;
    m_vertexBufferPtr = m_vertexBuffer;
    m_textureIndex = 0;
}

void SpriteBatch::SubmitTexturedQuad(Memory::Ptr<Texture2D> texture, const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) {
    CheckIndexes();
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 1 }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    std::array<glm::vec2, QUAD_COUNT> g_DefTexCoords = { glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f) };

    DrawIndexed(transform, m_vertexPositions, g_DefTexCoords, color, SetTexture(texture));
}

void SpriteBatch::SubmitTexturedQuad(Memory::Ptr<Texture2D> texture, const glm::mat4 &transform, const glm::vec4 &color) {
    CheckIndexes();
    std::array<glm::vec2, QUAD_COUNT> g_DefTexCoords = { glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f) };
    DrawIndexed(transform, m_vertexPositions, g_DefTexCoords, color, SetTexture(texture));
}

void SpriteBatch::SubmitTiledSprite(
    Memory::Ptr<Texture2D> texture, const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &pTileInfo, const glm::vec4 &color) {
    CheckIndexes();
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 1 }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    DrawIndexed(transform, m_vertexPositions, MakeTiled(texture, pTileInfo), color, SetTexture(texture));
}

void SpriteBatch::SubmitTiledSprite(Memory::Ptr<Texture2D> texture, const glm::mat4 &transform, const glm::vec4 &pTileInfo, const glm::vec4 &color) {
    CheckIndexes();
    DrawIndexed(transform, m_vertexPositions, MakeTiled(texture, pTileInfo), color, SetTexture(texture));
}

void SpriteBatch::CheckIndexes() {
    if (m_indexes >= _maxIndices)
        FlushReset();
}

void SpriteBatch::DrawIndexed(const glm::mat4 &transform, glm::vec4 vertexPos[QUAD_COUNT], const std::array<glm::vec2, QUAD_COUNT> &uvs, const glm::vec4 &color,
    float pTextureID, uint32_t pIndexCount) {
    for (size_t i = 0; i < QUAD_COUNT; i++) {
        m_vertexBufferPtr->pos = transform * vertexPos[i];
        m_vertexBufferPtr->color = color;
        m_vertexBufferPtr->texture_pos = uvs[i];
        m_vertexBufferPtr->batch_info = { pTextureID, 1 };
        //                                            ^ this is tiling, good for optimization
        m_vertexBufferPtr++;
    }
    m_indexes += pIndexCount;
    m_stats.quadCount++;
}

float SpriteBatch::SetTexture(Memory::Ptr<Texture2D> texture) {
    float textureIndex = 0.0f;
    for (uint32_t i = 0; i < m_textureIndex; i++) {
        if (*m_textureSlots[i] == *texture) {
            return i;
        }
    }

    if (textureIndex == 0.0f) {
        if (m_textureIndex >= _maxTextureSlots)
            FlushReset();

        textureIndex = (float)m_textureIndex;
        m_textureSlots[m_textureIndex] = texture;
        m_textureIndex++;
    }

    return textureIndex;
}

std::array<glm::vec2, QUAD_COUNT> SpriteBatch::MakeTiled(Memory::Ptr<Texture2D> texture, const glm::vec4 &pTileInfo) {
    float X = (1.f / texture->Width()) * pTileInfo.x;
    float Y = (1.f / texture->Height()) * pTileInfo.y;
    float W = (1.f / texture->Width()) * pTileInfo.z;
    float H = (1.f / texture->Height()) * pTileInfo.w;

    std::array<glm::vec2, QUAD_COUNT> ret;
    ret[0] = { X + W, Y + H };
    ret[1] = { X + W, Y };
    ret[2] = { X, Y };
    ret[3] = { X, Y + H };
    return ret;
}

BatchStats &SpriteBatch::GetBatchStats() {
    return m_stats;
}

void SpriteBatch::ResetStats() {
    memset(&m_stats, 0, sizeof(BatchStats));
}