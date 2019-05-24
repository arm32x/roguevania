#include "CollisionMap.hpp"

#include <algorithm>
#include <cstring>
#include <fstream>

#include <SFML/System/FileInputStream.hpp>

#include "../Exceptions/FileIOException.hpp"
#include "../Exceptions/FileNotFoundException.hpp"

using namespace RoguelikeMetroidvania;
using namespace RoguelikeMetroidvania::Collision;
using namespace sf;

bool CollisionMap::loadFromFile(const std::string& filename) {
    FileInputStream stream;
    if (!stream.open(filename)) return false;
    return loadFromStream(stream);
}

bool CollisionMap::loadFromStream(InputStream& stream) {
    uint8_t data[256];
    if (stream.read(data, 256) == -1) return false;
    return loadFromMemory(data);
}

bool CollisionMap::loadFromMemory(const void* data) {
    memcpy(&*buffer.begin(), data, 256);
    return true;
}

CollisionMode CollisionMap::operator[](uint8_t index) const {
    return buffer[index];
}