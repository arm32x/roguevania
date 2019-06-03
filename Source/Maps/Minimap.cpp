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

void Minimap::draw(RenderTarget& target, RenderStates states) const {
    View oldView = target.getView();
    target.setView(view);
    Tilemap::draw(target, states);
    target.setView(oldView);
}
