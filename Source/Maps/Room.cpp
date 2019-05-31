#include "Room.hpp"

#include "../Maps/Tilemap.hpp"

using namespace RoguelikeMetroidvania;
using namespace RoguelikeMetroidvania::Maps;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreorder"

Room::Room(Section* section, uint8_t doors, uint8_t openings)
    : section(section), doors(doors), openings(openings) {
    
}

Room::Room(uint8_t doors, uint8_t openings)
    : Room(nullptr, doors, openings) {
    
}

Room::Room(Section* section, uint8_t combined)
    : Room(section, combined & 0b00001111, combined >> 4 & 0b00001111) {
    
}

Room::Room(uint8_t combined)
    : Room(nullptr, combined) {
    
}

Room::Room(Section* section)
    : Room(section, 0b00000000) {
    
}

Room::Room()
    : Room(nullptr) {
    
}

#pragma GCC diagnostic pop

Room::~Room() {
    delete tilemap;
}