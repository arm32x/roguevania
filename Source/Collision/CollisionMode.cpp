#include "CollisionMode.hpp"

#include <iomanip>

#include <SFML/Window/Keyboard.hpp>

#include "../Program.hpp"

using namespace Roguevania;
using namespace Roguevania::Collision;
using namespace sf;

CollisionMode::CollisionMode()
    : value(CollisionMode::Undefined) {
    
}

CollisionMode::CollisionMode(const CollisionMode::Value& value)
    : value(value) {
    
}

bool CollisionMode::operator==(const CollisionMode::Value& other) const { return value == other; }
bool CollisionMode::operator==(const CollisionMode&        other) const { return value == other.value; }
bool CollisionMode::operator!=(const CollisionMode::Value& other) const { return value != other; }
bool CollisionMode::operator!=(const CollisionMode&        other) const { return value != other.value; }

#define DBG if (Keyboard::isKeyPressed(Keyboard::Tilde))
CollisionMode CollisionMode::prioritize(const std::vector<CollisionMode>& modes) {
    if (modes.size() <= 0) return CollisionMode::NotTouching;
    CollisionMode result = CollisionMode::NotTouching;
    for (CollisionMode mode : modes) {
        DBG Program::log(Log::Debug, "CollisionMode") << "Current champion is " << result << ".  Challenger is " << mode << "." << std::endl;
        if (result.value >= 0xF0 && mode.value < 0xF0) {
            DBG Program::log(Log::Debug, "CollisionMode") << "Challenger succeeded because champion was >= F0." << std::endl;
            result.value = mode.value;
        } else if (mode.value < 0xF0 && result.value < mode.value) {
            DBG Program::log(Log::Debug, "CollisionMode") << "Challenger succeeded because champion < challenger." << std::endl;
            result.value = mode.value;
        }
    }
    DBG Program::log(Log::Debug, "CollisionMode") << "Final champion is " << result << "." << std::endl;
    return result;
}

CollisionMode::operator uint8_t() const {
    return static_cast<uint8_t>(value);
}

std::ostream& operator<<(std::ostream& stream, const CollisionMode& mode) {
    return stream << std::hex << std::uppercase << +mode << std::dec << std::nouppercase;
}

std::ostream& operator<<(std::ostream& stream, const CollisionMode::Value& mode) {
    return stream << std::hex << std::uppercase << +mode << std::dec << std::nouppercase;
}