#include "pch.h"

#include "Tilesheet.h"

#include "Utils/Logger.h"
#include "Utils/FileSystem.h"

using namespace IceSDK::Graphics;

Tilesheet::Tilesheet(Memory::Ptr<Texture2D> texture, const char *infoPath) {
    /*int numX, numY = 0;

    numX = texture->Width() / size;
    numY = texture->Height() / size;

    for (int x = 0; x < numX; x++) {
        for (int y = 0; y < numY; y++) {
            Tile t;
            t.id = m_tiles.size();
            t.coordinates = { x * size, y * size };
        }
    }*/
}

Tilesheet::~Tilesheet() {
}

Tile Tilesheet::GetTile(uint32_t pID) {
    if (this->m_tiles.size() > pID) {
        return m_tiles[pID];
    }
    return {};
}

// shared ptr due to intellisense
std::shared_ptr<Texture2D> Tilesheet::GetTexture() {
    return m_texture;
}

uint32_t Tilesheet::TileSize() {
    return m_tiles.size();
}