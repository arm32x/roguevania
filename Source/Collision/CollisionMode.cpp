#include "CollisionMode.hpp"

#include <iomanip>

#include "../Program.hpp"

using namespace Roguevania;
using namespace Roguevania::Collision;

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

CollisionMode CollisionMode::prioritize(const std::vector<CollisionMode>& modes) {
    if (modes.size() <= 0) return CollisionMode::NotTouching;
    CollisionMode result = CollisionMode::NotTouching;
    Program::log(Log::Debug, "CollisionMode") << "Prioritizing " << modes.size() << " collision modes..." << std::endl;
    for (CollisionMode mode : modes) {
        if (result.value >= 0xF0 && mode.value < 0xF0) {
            result.value = mode.value;
        } else if (result.value < mode.value) {
            result.value = mode.value;
        }
    }
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