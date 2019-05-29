#include "CollisionMap.hpp"

#include <algorithm>
#include <cstring>
#include <fstream>

#include <SFML/System/FileInputStream.hpp>

#include "../Exceptions/FileIOException.hpp"
#include "../Exceptions/FileNotFoundException.hpp"
#include "../Program.hpp"

using namespace RoguelikeMetroidvania;
using namespace RoguelikeMetroidvania::Collision;
using namespace sf;

void CollisionMap::loadFromFile(const char* filename) {
    Program::log(Log::Trace, "CollisionMap") << "Loading collision map from file '" << filename << "'..." << std::endl;
    std::ifstream stream;
    stream.open(filename, std::ios::in | std::ios::binary);
    if (stream.fail()) {
        Program::log(Log::Error, "CollisionMap") << "Could not open file '" << filename << "'." << std::endl;
        throw Exceptions::FileNotFoundException("Could not open file.");
    }
    loadFromStream(stream);
}

void CollisionMap::loadFromStream(std::istream& stream) {
    Program::log(Log::Trace, "CollisionMap") << "Loading collision map from input stream..." << std::endl;
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
    Program::log(Log::Trace, "CollisionMap") << "Loading collision map from memory..." << std::endl;
    memcpy(&*buffer.begin(), data, 256);
    Program::log(Log::Trace, "CollisionMap") << "Done." << std::endl;
}

CollisionMode CollisionMap::operator[](uint8_t index) const {
    return buffer[index];
}