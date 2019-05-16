#pragma once

#include <array>
#include <cstdint>
#include <string>

#include <SFML/System/InputStream.hpp>

namespace RoguelikeMetroidvania {
namespace Maps {
using namespace sf;

class CollisionMap {
        
    public:
        enum Mode : uint8_t {
            Undefined = 0xFF,
            None      = 0x00,
            Solid     = 0x01,
        };
        
        bool loadFromFile(const std::string& filename);
        bool loadFromStream(InputStream& stream);
        bool loadFromMemory(const void* data);
        
        Mode operator[](uint8_t index) const;
        
        std::array<Mode, 256> buffer;
        
};

};
};