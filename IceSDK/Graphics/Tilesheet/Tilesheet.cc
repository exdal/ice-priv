#include "pch.h"

#include "Tilesheet.h"

#include "Utils/Logger.h"

using namespace IceSDK::Graphics;

Tilesheet::Tilesheet(Memory::Ptr<Texture2D> pTexture, int pSize) {
    int numX, numY = 0;

    numX = pTexture->Width() / pSize;
    numY = pTexture->Height() / pSize;

    for (int y = 0; y < numY; y++) {
        for (int x = 0; x < numX; x++) { _textureCoordinates.push_back({ x * pSize, y * pSize, pSize, pSize }); }
    }
}

Tilesheet::~Tilesheet() {
}

glm::vec4 Tilesheet::GetTile(uint32_t pID) {
    if (this->_textureCoordinates.size() > pID) {
        return _textureCoordinates[pID];
    }
    return glm::vec4(0, 0, 0, 0);
}

// shared ptr due to intellisense
std::shared_ptr<Texture2D> Tilesheet::GetTexture() {
    return this->_texture;
}

uint32_t Tilesheet::TileSize() {
    return this->_textureCoordinates.size();
}