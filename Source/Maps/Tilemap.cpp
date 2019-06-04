#include "Tilemap.hpp"

#include <fstream>

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/FileInputStream.hpp>
#include <SFML/System/Vector2.hpp>

#include "../Exceptions/FileIOException.hpp"
#include "../Exceptions/FileNotFoundException.hpp"
#include "../Program.hpp"

using namespace Roguevania;
using namespace Roguevania::Maps;
using namespace sf;

Tilemap::Tilemap(uint16_t width, uint16_t height, uint32_t tileSize, const char* graphicsFilename)
    : width(width), height(height), tileSize(!graphics.loadFromFile(graphicsFilename) ? throw Exceptions::FileIOException("Could not load texture for tilemap.") : tileSize) {
    std::cout << graphics.getNativeHandle() << std::endl;
    for (uint16_t y = 0; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            setTileType(x, y, 0x00);
        }
    }
}

Tilemap::Tilemap(uint16_t width, uint16_t height, const char* graphicsFilename)
    : width(width), height(height), tileSize(!graphics.loadFromFile(graphicsFilename) ? throw Exceptions::FileIOException("Could not load texture for tilemap.") : (graphics.getSize().x / 16)) {
    for (uint16_t y = 0; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            setTileType(x, y, 0x00);
        }
    }
}

uint8_t Tilemap::getTileType(uint16_t x, uint16_t y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) return 0;
    const Vertex& vertex = vertices[(x + width * y) * 4];
    return (vertex.texCoords.y / tileSize) * 16 + (vertex.texCoords.x / tileSize);
}

void Tilemap::setTileType(uint16_t x, uint16_t y, uint8_t type) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    vertices[(x + width * y) * 4 + 0].position = Vector2f(x * tileSize, y * tileSize);
    vertices[(x + width * y) * 4 + 1].position = Vector2f(x * tileSize + tileSize, y * tileSize);
    vertices[(x + width * y) * 4 + 2].position = Vector2f(x * tileSize + tileSize, y * tileSize + tileSize);
    vertices[(x + width * y) * 4 + 3].position = Vector2f(x * tileSize, y * tileSize + tileSize);
    vertices[(x + width * y) * 4 + 0].texCoords = Vector2f((type % 16) * tileSize, std::floor(type / 16) * tileSize);
    vertices[(x + width * y) * 4 + 1].texCoords = Vector2f((type % 16) * tileSize + tileSize, std::floor(type / 16) * tileSize);
    vertices[(x + width * y) * 4 + 2].texCoords = Vector2f((type % 16) * tileSize + tileSize, std::floor(type / 16) * tileSize + tileSize);
    vertices[(x + width * y) * 4 + 3].texCoords = Vector2f((type % 16) * tileSize, std::floor(type / 16) * tileSize + tileSize);
}

const Color& Tilemap::getTileColor(uint16_t x, uint16_t y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) return Color::Black;
    return vertices[(x + width * y) * 4].color;
}

void Tilemap::setTileColor(uint16_t x, uint16_t y, const Color& color) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    vertices[(x + width * y) * 4 + 0].color = color;
    vertices[(x + width * y) * 4 + 1].color = color;
    vertices[(x + width * y) * 4 + 2].color = color;
    vertices[(x + width * y) * 4 + 3].color = color;
}

void Tilemap::loadFromFile(const char* filename) {
    std::ifstream stream;
    stream.open(filename, std::ios::in | std::ios::binary);
    if (stream.fail()) {
        Program::log(Log::Error, "Tilemap") << "Could not open file '" << filename << "'." << std::endl;
        throw Exceptions::FileNotFoundException("Could not open file.");
    }
    loadFromStream(stream);
}

void Tilemap::loadFromStream(std::istream& stream) {
    std::vector<uint8_t> data;
    data.resize(width * height);
    stream.read(reinterpret_cast<char*>(&*data.begin()), width * height);
    if (!stream.good()) {
        if (stream.eof()) {
            Program::log(Log::Error, "Tilemap") << "Could not read data, reached EOF prematurely." << std::endl;
            throw Exceptions::FileIOException("Could not read data, reached EOF prematurely.");
        } else {
            Program::log(Log::Error, "Tilemap") << "An unexpected error occured while reading data." << std::endl;
            throw Exceptions::FileIOException("An unexpected error occured while reading data.");
        }
    }
    loadFromMemory(&*data.begin());
}

void Tilemap::loadFromMemory(const uint8_t* data) {
    for (uint16_t y = 0; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            setTileType(x, y, data[x + width * y]);
        }
    }
}

void Tilemap::draw(RenderTarget& target, RenderStates states) const {
    states.transform *= getTransform();
    states.texture = &graphics;
    target.draw(vertices, states);
}