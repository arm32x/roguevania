#include "Minimap.hpp"

#include <cmath>
#include <iostream>

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "../Program.hpp"

using namespace Roguevania;
using namespace Roguevania::Maps;
using namespace sf;

Minimap::Minimap(uint16_t width, uint16_t height)
    : Tilemap(width, height, 8, "Resources/Tilemaps/Minimap.png"), view(FloatRect(0, 0, 56, 56)) {
    view.setViewport(FloatRect(1.0 - (56.0 + 8.0) / 640.0, 8.0 / 360.0, 56.0 / 640.0, 56.0 / 360.0));
}

void Minimap::setOverlayPosition(uint32_t id, uint16_t x, uint16_t y) {
    if (vertices.getVertexCount() < width * height * 4 + (id + 1) * 4) vertices.resize(width * height * 4 + (id + 1) * 4);
    vertices[width * height * 4 + id * 4 + 0].position = Vector2f(x * tileSize, y * tileSize);
    vertices[width * height * 4 + id * 4 + 1].position = Vector2f(x * tileSize + tileSize, y * tileSize);
    vertices[width * height * 4 + id * 4 + 2].position = Vector2f(x * tileSize + tileSize, y * tileSize + tileSize);
    vertices[width * height * 4 + id * 4 + 3].position = Vector2f(x * tileSize, y * tileSize + tileSize);
}

void Minimap::setOverlayType(uint32_t id, uint8_t type) {
    if (vertices.getVertexCount() < width * height * 4 + (id + 1) * 4) vertices.resize(width * height * 4 + (id + 1) * 4);
    vertices[width * height * 4 + id * 4 + 0].texCoords = Vector2f((type % 16) * tileSize, std::floor(type / 16) * tileSize + tileSize * 16);
    vertices[width * height * 4 + id * 4 + 1].texCoords = Vector2f((type % 16) * tileSize + tileSize, std::floor(type / 16) * tileSize + tileSize * 16);
    vertices[width * height * 4 + id * 4 + 2].texCoords = Vector2f((type % 16) * tileSize + tileSize, std::floor(type / 16) * tileSize + tileSize + tileSize * 16);
    vertices[width * height * 4 + id * 4 + 3].texCoords = Vector2f((type % 16) * tileSize, std::floor(type / 16) * tileSize + tileSize + tileSize * 16);
}

void Minimap::setOverlayColor(uint32_t id, const Color& color) {
    
}

void Minimap::draw(RenderTarget& target, RenderStates states) const {
    static Vertex background[4] {
        Vertex(Vector2f( 0.0f,  0.0f), Color::Black),
        Vertex(Vector2f(56.0f,  0.0f), Color::Black),
        Vertex(Vector2f(56.0f, 56.0f), Color::Black),
        Vertex(Vector2f( 0.0f, 56.0f), Color::Black)
    };
    View oldView = target.getView();
    target.setView(view);
    target.draw(background, 4, PrimitiveType::Quads, states);
    Tilemap::draw(target, states);
    target.setView(oldView);
}
