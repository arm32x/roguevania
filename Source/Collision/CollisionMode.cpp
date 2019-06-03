#include "CollisionMode.hpp"

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
    CollisionMode result = CollisionMode::NotTouching;
    for (CollisionMode mode : modes) {
        if (result.value >= 0xF0 && mode.value < 0xF0) {
            result.value = mode.value;
        } else if (result.value < mode.value) {
            result.value = mode.value;
        } else {
        }
    }
    return result;
}

CollisionMode::operator uint8_t() const {
    return static_cast<uint8_t>(value);
}