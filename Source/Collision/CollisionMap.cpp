#include "CollisionMap.hpp"

#include <algorithm>
#include <cstring>
#include <fstream>

#include <SFML/System/FileInputStream.hpp>

#include "../Exceptions/FileIOException.hpp"
#include "../Exceptions/FileNotFoundException.hpp"
#include "../Program.hpp"

using namespace Roguevania;
using namespace Roguevania::Collision;
using namespace sf;

void CollisionMap::loadFromFile(const char* filename) {
    std::ifstream stream;
    stream.open(filename, std::ios::in | std::ios::binary);
    if (stream.fail()) {
        Program::log(Log::Error, "CollisionMap") << "Could not open file '" << filename << "'." << std::endl;
        throw Exceptions::FileNotFoundException("Could not open file.");
    }
    loadFromStream(stream);
}

void CollisionMap::loadFromStream(std::istream& stream) {
    uint8_t data[256];
    stream.read(reinterpret_cast<char*>(data), sizeof(data));
    if (!stream.good()) {
        if (stream.eof()) {
            Program::log(Log::Error, "CollisionMap") << "Could not read data, reached EOF prematurely." << std::endl;
            throw Exceptions::FileIOException("Could not read data, reached EOF prematurely.");
        } else {
            Program::log(Log::Error, "CollisionMap") << "An unexpected error occured while reading data." << std::endl;
            throw Exceptions::FileIOException("An unexpected error occured while reading data.");
        }
    }
    loadFromMemory(data);
}

void CollisionMap::loadFromMemory(const uint8_t* data) {
    memcpy(&*buffer.begin(), data, 256);
}

CollisionMode CollisionMap::operator[](uint8_t index) const {
    return buffer[index];
}