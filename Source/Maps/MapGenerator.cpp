#include "MapGenerator.hpp"

#include <bitset>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <unordered_map>

#include <ghc/filesystem.hpp>
#include <stx/optional.hpp>

#include "../Exceptions/FileNotFoundException.hpp"
#include "../Exceptions/ParseException.hpp"
#include "../Maps/Room.hpp"
#include "../Maps/Section.hpp"
#include "../Platform/Entity.hpp"
#include "../Platform/SineWaveEnemy.hpp"
#include "../Utilities/HSVtoRGB.hpp"
#include "../Program.hpp"

using namespace Roguevania;
using namespace Roguevania::Maps;
using namespace Roguevania::Platform;
namespace ghcfs = ghc::filesystem;
using stx::nullopt;

MapGenerator::MapGenerator(uint64_t seed)
    : random(seed), seed(seed) {
    
}

MapGenerator::MapGenerator()
    : random() {
    setSeed(random.engine()());
}

void MapGenerator::setSeed(uint64_t seed) {
    this->seed = seed;
    random.seed(seed);
}

Room MapGenerator::generateRoom() {
    uint8_t combined = 0b00000000;
    combined |= static_cast<bool>(random.uniform(0, 1)) << 0;
    combined |= static_cast<bool>(random.uniform(0, 1)) << 2;
    combined |= static_cast<bool>(random.uniform(0, 1)) << 1;
    combined |= static_cast<bool>(random.uniform(0, 1)) << 3;
    return Room(combined);
}

std::deque<Room> MapGenerator::generateRooms(uint16_t width, uint16_t height) {
    std::deque<Room> result;
    for (uint16_t y = 0; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            Room generatedRoom = generateRoom();
            generatedRoom.x = x;
            generatedRoom.y = y;
            result.push_back(generatedRoom);
        }
    }
    connectRooms(result, width, height);
    return result;
}

std::deque<Section> MapGenerator::generateSections(std::deque<Room>& rooms, uint16_t width, uint16_t height, uint32_t minimumSectionSize) {
    std::deque<Room*> fillQueue;
    std::deque<Section> sections;
    std::deque<Room*> filledRooms;
    {
        uint16_t x, y;
        do {
            x = random.uniform(0, width - 1);
            y = random.uniform(0, height - 1);
        } while (rooms[x + width * y].doors == 0b0000);
        fillQueue.push_back(&rooms[x + width * y]);
    }
    constexpr float golden = 0.618033988749895;
    float hue = random.uniform(0.0, 1.0);
    for (char sectionID = 'A'; true; sectionID++) {
        sections.emplace_front(sectionID, Utilities::HSVtoRGB(hue, 0.75, 0.65));
        Section& section = sections.front();
        expandSection: while (!fillQueue.empty()) {
            Room& r = *fillQueue.front();
            fillQueue.pop_front();
            if (r.section == nullptr) {
                r.section = &section;
                section.rooms.push_front(&r);
                filledRooms.push_front(&r);
            }
            if ((r.doors & 0b0001) != 0 && rooms[(r.x - 1) + width * r.y].section == nullptr) fillQueue.push_back(&rooms[(r.x - 1) + width * r.y]);
            if ((r.doors & 0b0010) != 0 && rooms[r.x + width * (r.y + 1)].section == nullptr) fillQueue.push_back(&rooms[r.x + width * (r.y + 1)]);
            if ((r.doors & 0b0100) != 0 && rooms[(r.x + 1) + width * r.y].section == nullptr) fillQueue.push_back(&rooms[(r.x + 1) + width * r.y]);
            if ((r.doors & 0b1000) != 0 && rooms[r.x + width * (r.y - 1)].section == nullptr) fillQueue.push_back(&rooms[r.x + width * (r.y - 1)]);
        }
        startSection: {
            std::deque<Room*>& roomsAvailable = (section.rooms.size() < minimumSectionSize && !section.rooms.empty()) ? section.rooms : filledRooms;
            for (Room* rptr : roomsAvailable) {
                Room& r = *rptr;
                if (r.x >= 1          && (r.doors & 0b0001) == 0 && rooms[(r.x - 1) + width * r.y].doors != 0b0000 && rooms[(r.x - 1) + width * r.y].section == nullptr) {
                    if (section.rooms.size() < minimumSectionSize && !section.rooms.empty()) {
                        r.doors |= 0b0001;
                        rooms[(r.x - 1) + width * r.y].doors |= 0b0100;
                    }
                    fillQueue.push_back(&rooms[(r.x - 1) + width * r.y]);
                    break;
                }
                if (r.y <= height - 2 && (r.doors & 0b0010) == 0 && rooms[r.x + width * (r.y + 1)].doors != 0b0000 && rooms[r.x + width * (r.y + 1)].section == nullptr) {
                    if (section.rooms.size() < minimumSectionSize && !section.rooms.empty()) {
                        r.doors |= 0b0010;
                        rooms[r.x + width * (r.y + 1)].doors |= 0b1000;
                    }
                    fillQueue.push_back(&rooms[r.x + width * (r.y + 1)]);
                    break;
                }
                if (r.x <= width - 2  && (r.doors & 0b0100) == 0 && rooms[(r.x + 1) + width * r.y].doors != 0b0000 && rooms[(r.x + 1) + width * r.y].section == nullptr) {
                    if (section.rooms.size() < minimumSectionSize && !section.rooms.empty()) {
                        r.doors |= 0b0100;
                        rooms[(r.x + 1) + width * r.y].doors |= 0b0001;
                    }
                    fillQueue.push_back(&rooms[(r.x + 1) + width * r.y]);
                    break;
                }
                if (r.y >= 1          && (r.doors & 0b1000) == 0 && rooms[r.x + width * (r.y - 1)].doors != 0b0000 && rooms[r.x + width * (r.y - 1)].section == nullptr) {
                    if (section.rooms.size() < minimumSectionSize && !section.rooms.empty()) {
                        r.doors |= 0b1000;
                        rooms[r.x + width * (r.y - 1)].doors |= 0b0010;
                    }
                    fillQueue.push_back(&rooms[r.x + width * (r.y - 1)]);
                    break;
                }
            }
            if (roomsAvailable == filledRooms && fillQueue.empty()) break;
        }
        if (!fillQueue.empty() && section.rooms.size() < minimumSectionSize) {
            goto expandSection;
        } else if (fillQueue.empty() && section.rooms.size() < minimumSectionSize) {
            for (Room* rptr : section.rooms) {
                rptr->doors = 0b0000;
                rptr->section = nullptr;
            }
            section.rooms.clear();
            goto startSection;
            goto expandSection;
        }
        hue += golden;
        hue = std::fmod(hue, 1.0f);
    }
    connectSections(rooms, width, height);
    return sections;
}

void MapGenerator::connectRooms(std::deque<Room>& rooms, uint16_t width, uint16_t height) {
    for (uint16_t y = 0; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            uint8_t doors = rooms[x + width * y].doors;
            {
                // Make sure that no rooms are connected to the edge of the map.
                if (x <= 0)          doors &= ~0b0001;
                if (y >= height - 1) doors &= ~0b0010;
                if (x >= width - 1)  doors &= ~0b0100;
                if (y <= 0)          doors &= ~0b1000;
            }
            if (static_cast<bool>(random.uniform(0, 1))) {
                // Connect two rooms when only one has an exit on a side.
                if ((doors & 0b0001) == 0 && x > 0          && (rooms[(x - 1) + width * y].doors & 0b0100) != 0) doors |=  0b0001;
                if ((doors & 0b0010) == 0 && y < height - 1 && (rooms[x + width * (y + 1)].doors & 0b1000) != 0) doors |=  0b0010;
                if ((doors & 0b0100) == 0 && x < width - 1  && (rooms[(x + 1) + width * y].doors & 0b0001) != 0) doors |=  0b0100;
                if ((doors & 0b1000) == 0 && y > 0          && (rooms[x + width * (y - 1)].doors & 0b0010) != 0) doors |=  0b1000;
            }
            {
                // Disconnect two rooms when only one has an exit on a side.
                if ((doors & 0b0001) != 0 && x > 0          && (rooms[(x - 1) + width * y].doors & 0b0100) == 0) doors &= ~0b0001;
                if ((doors & 0b0010) != 0 && y < height - 1 && (rooms[x + width * (y + 1)].doors & 0b1000) == 0) doors &= ~0b0010;
                if ((doors & 0b0100) != 0 && x < width - 1  && (rooms[(x + 1) + width * y].doors & 0b0001) == 0) doors &= ~0b0100;
                if ((doors & 0b1000) != 0 && y > 0          && (rooms[x + width * (y - 1)].doors & 0b0010) == 0) doors &= ~0b1000;
            }
            rooms[x + width * y].doors = doors;
        }
    }
    // TODO:  Implement smarter camera code to allow for this.
    #if FALSE
    for (uint16_t y = 0; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            {
                // Replace “ring” room formations with large square rooms.
                if (                                      (rooms[(x    ) + width * (y    )].openings) != 0b0000) continue;
                if (x >= width - 1  ||                    (rooms[(x + 1) + width * (y    )].openings) != 0b0000) continue;
                if (                   y >= height - 1 || (rooms[(x    ) + width * (y + 1)].openings) != 0b0000) continue;
                if (x >= width - 1  || y >= height - 1 || (rooms[(x + 1) + width * (y + 1)].openings) != 0b0000) continue;
                if (                                      (rooms[(x    ) + width * (y    )].doors & 0b0110) != 0b0110) continue;
                if (x >= width - 1  ||                    (rooms[(x + 1) + width * (y    )].doors & 0b0011) != 0b0011) continue;
                if (                   y >= height - 1 || (rooms[(x    ) + width * (y + 1)].doors & 0b1100) != 0b1100) continue;
                if (x >= width - 1  || y >= height - 1 || (rooms[(x + 1) + width * (y + 1)].doors & 0b1001) != 0b1001) continue;
                rooms[(x    ) + width * (y    )].openings |= 0b0110;
                rooms[(x + 1) + width * (y    )].openings |= 0b0011;
                rooms[(x    ) + width * (y + 1)].openings |= 0b1100;
                rooms[(x + 1) + width * (y + 1)].openings |= 0b1001;
            }
        }
    }
    #endif
}

// TODO:  Make at least one connection between sequential sections (‘A’ and ‘B’).
void MapGenerator::connectSections(std::deque<Room>& rooms, uint16_t width, uint16_t height) {
    for (uint16_t y = 0; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            if (rooms[(x    ) + width * (y    )].section == nullptr) continue;
            if (x > 0          && rooms[(x - 1) + width * (y    )].section != nullptr && rooms[(x - 1) + width * (y    )].section < rooms[(x    ) + width * (y    )].section && random.uniform(0, 1) == 0) {
                rooms[(x    ) + width * (y    )].doors |= 0b0001;
                rooms[(x - 1) + width * (y    )].doors |= 0b0100;
                continue;
            }
            if (y < height - 1 && rooms[(x    ) + width * (y + 1)].section != nullptr && rooms[(x    ) + width * (y + 1)].section < rooms[(x    ) + width * (y    )].section && random.uniform(0, 1) == 0) {
                rooms[(x    ) + width * (y    )].doors |= 0b0010;
                rooms[(x    ) + width * (y + 1)].doors |= 0b1000;
                continue;
            }
            if (x < width - 1  && rooms[(x + 1) + width * (y    )].section != nullptr && rooms[(x + 1) + width * (y    )].section < rooms[(x    ) + width * (y    )].section && random.uniform(0, 1) == 0) {
                rooms[(x    ) + width * (y    )].doors |= 0b0100;
                rooms[(x + 1) + width * (y    )].doors |= 0b0001;
                continue;
            }
            if (y > 0          && rooms[(x    ) + width * (y - 1)].section != nullptr && rooms[(x    ) + width * (y - 1)].section < rooms[(x    ) + width * (y    )].section && random.uniform(0, 1) == 0) {
                rooms[(x    ) + width * (y    )].doors |= 0b1000;
                rooms[(x    ) + width * (y - 1)].doors |= 0b0010;
                continue;
            }
        }
    }
}

MapGenerator::Random& MapGenerator::getRandom() {
    return random;
}

void MapGenerator::generateRoomLayout(Room& room) {
    std::vector<ghcfs::path> candidates;
    for (const ghcfs::directory_entry& dir : ghcfs::directory_iterator("Resources/Rooms/")) {
        if (dir.is_directory()) continue;
        std::string filename = dir.path().stem();
        if (filename[0] != ((room.openings & 0b1000) != 0) + '0') continue;
        if (filename[1] != ((room.openings & 0b0100) != 0) + '0') continue;
        if (filename[2] != ((room.openings & 0b0010) != 0) + '0') continue;
        if (filename[3] != ((room.openings & 0b0001) != 0) + '0') continue;
        if (filename[4] != ((room.doors    & 0b1000) != 0) + '0') continue;
        if (filename[5] != ((room.doors    & 0b0100) != 0) + '0') continue;
        if (filename[6] != ((room.doors    & 0b0010) != 0) + '0') continue;
        if (filename[7] != ((room.doors    & 0b0001) != 0) + '0') continue;
        candidates.push_back(dir.path());
    }
    if (candidates.size() == 0) candidates.push_back("Resources/Rooms/special_00001111_undefined.rrm");
    generateRoomLayoutFromFile(room, random.pick(candidates).c_str());
}

void MapGenerator::generateRoomLayoutFromFile(Room& room, const char* filename) {
    std::ifstream stream;
    stream.open(filename, std::ios::in | std::ios::binary);
    if (stream.fail()) {
        Program::log(Log::Error, "MapGenerator") << "Could not open room template file '" << filename << "'." << std::endl;
        throw Exceptions::FileNotFoundException("Could not open room template file.");
    }
    generateRoomLayoutFromStream(room, stream);
}

void MapGenerator::generateRoomLayoutFromStream(Room& room, std::istream& stream) {
    std::unordered_map<uint16_t, std::vector<uint8_t>> pools;
    std::unordered_map<uint16_t, uint8_t> instances;
    char c;
    while (stream.get(c)) {
        switch (c) {
            case 'R':
                stream.get(c);
                switch (c) {
                    case 'R':
                        if (stream.peek() != 'M') {
                            Program::log(Log::Error, "MapGenerator") << "Room layout file is malformed, invalid section header (expected 'RRM', found 'RR" << stream.peek() << "')." << std::endl;
                            throw Exceptions::ParseException("Room layout file is malformed, invalid section header.");
                        }
                        if (room.tilemap != nullopt) {
                            Program::log(Log::Warning, "MapGenerator") << "Room already contains a tilemap, which will be reused and assumed to be of the right size." << std::endl;
                            stream.ignore(14);
                            continue;
                        }
                        
                        stream.ignore(10);
                        
                        uint16_t width, height;
                        stream.read(reinterpret_cast<char*>(&width),  2);
                        stream.read(reinterpret_cast<char*>(&height), 2);
                        
                        room.tilemap.emplace(width, height, 16, "Resources/Tilemaps/Default.png");
                        
                        break;
                    case 'M':
                        if (stream.peek() != 'D') {
                            Program::log(Log::Error, "MapGenerator") << "Room layout file is malformed, invalid section header (expected 'RMD', found 'RM" << stream.peek() << "')." << std::endl;
                            throw Exceptions::ParseException("Room layout file is malformed, invalid section header.");
                        }
                        if (room.tilemap == nullopt) {
                            Program::log(Log::Error, "MapGenerator") << "Room layout file should have exactly one 'RRM' section before an 'RMD' section." << std::endl;
                            throw Exceptions::ParseException("Room layout file should have exactly one 'RRM' section before an 'RMD' section.");
                        }
                        
                        stream.ignore(14);
                        
                        for (uint16_t y = 0; y < room.tilemap->height; y++) {
                            for (uint16_t x = 0; x < room.tilemap->width; x++) {
                                uint16_t poolID, instanceNo;
                                stream.read(reinterpret_cast<char*>(&poolID),     2);
                                stream.read(reinterpret_cast<char*>(&instanceNo), 2);
                                try {
                                    if (instanceNo != 0xFFFF) {
                                        auto it = instances.find(instanceNo);
                                        if (it != instances.end()) {
                                            (void)pools.at(poolID); // Trigger exception if pool ID does not exist.
                                            try {
                                                room.tilemap->setTileType(x, y, pools[poolID].at(it->second));
                                            } catch (const std::out_of_range& ex) {
                                                Program::log(Log::Error, "MapGenerator") << "Instance ID " << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << +reinterpret_cast<char*>(&instanceNo)[0] << +reinterpret_cast<char*>(&instanceNo)[1] << std::dec << std::nouppercase << std::setfill(' ') << std::setw(0) << " used for a large pool is subsequently used for a smaller pool.  Instances should be used with pools of the same size for intended results." << std::endl;
                                                throw Exceptions::ParseException("Instance ID used for a large pool is subsequently used for a smaller pool.");
                                            }
                                        } else {
                                            uint8_t index = random.uniform(0u, pools.at(poolID).size() - 1);
                                            instances.emplace(instanceNo, index);
                                            room.tilemap->setTileType(x, y, pools.at(poolID)[index]);
                                        }
                                    } else {
                                        uint8_t tile = random.pick(pools.at(poolID));
                                        room.tilemap->setTileType(x, y, tile);
                                    }
                                } catch (const std::out_of_range& ex) {
                                    Program::log(Log::Error, "MapGenerator") << "Pool " << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << +reinterpret_cast<char*>(&poolID)[0] << +reinterpret_cast<char*>(&poolID)[1] << std::dec << std::nouppercase << std::setfill(' ') << std::setw(0) << " does not exist." << std::endl;
                                }
                            }
                        }
                        return;
                        break;
                }
                break;
            case 'P':
                uint16_t poolID;
                stream.read(reinterpret_cast<char*>(&poolID), 2);
                uint8_t length;
                stream.read(reinterpret_cast<char*>(&length), 1);
                
                pools[poolID].reserve(length);
                for (int index = 0; index < length; index++) {
                    pools.at(poolID).push_back(stream.get());
                }
                
                stream.ignore((8 - (length + 4) % 8) % 8);
                break;
            case 'E':
                if (room.tilemap == nullopt) {
                    Program::log(Log::Error, "MapGenerator") << "Room layout file should have exactly one 'RRM' section before an 'E' section." << std::endl;
                    throw Exceptions::ParseException("Room layout file should have exactly one 'RRM' section before an 'E' section.");
                }
                uint8_t enemyType;
                stream.read(reinterpret_cast<char*>(&enemyType), 1);
                switch (enemyType) {
                    case 0x01: { // Horizontal flying enemy.
                        float x, y;
                        stream.read(reinterpret_cast<char*>(&x), 4);
                        stream.read(reinterpret_cast<char*>(&y), 4);
                        
                        int8_t distanceX, distanceY, speedX, speedY;
                        stream.read(reinterpret_cast<char*>(&distanceX), 1);
                        stream.read(reinterpret_cast<char*>(&distanceY), 1);
                        stream.read(reinterpret_cast<char*>(&speedX), 1);
                        stream.read(reinterpret_cast<char*>(&speedY), 1);
                        Vector2f distance((distanceX < 0 ? -1 : 1) * std::pow(2.0f, std::abs(distanceX)), (distanceY < 0 ? -1 : 1) * std::pow(2.0f, std::abs(distanceY)));
                        Vector2f speed(std::pow(2.0f, std::abs(speedX)), std::pow(2.0f, std::abs(speedY)));
                        
                        room.entities.push_back(new SineWaveEnemy(Entity::spritesheet, IntRect(32, 16, 16, 16), (room.x * room.tilemap->width + x) * room.tilemap->tileSize, (room.y * room.tilemap->height + y) * room.tilemap->tileSize, speed, distance));
                        
                        stream.ignore(2);
                        break;
                    }
                    default: {
                        Program::log(Log::Error, "MapGenerator") << "Undefined entity type " << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << +enemyType << std::dec << std::nouppercase << std::setfill(' ') << std::setw(0) << "." << std::endl;
                        throw Exceptions::ParseException("Undefined entity type.");
                        break;
                    }
                }
                break;
            case '\xFF':
                stream.ignore(7);
                break;
            default:
                Program::log(Log::Error, "MapGenerator") << "Room layout file is malformed, invalid section header (found '" << c << "')." << std::endl;
                throw Exceptions::ParseException("Room layout file is malformed, invalid section header.");
                break;
        }
    }
    // The 'RMD' section handler returns the generated room layout.  If this is reached, there are no 'RMD' sections.
    Program::log(Log::Error, "MapGenerator") << "Room layout file must have exactly one 'RMD' section at the end of the file." << std::endl;
    throw Exceptions::ParseException("Room layout file must have exactly one 'RMD' section at the end of the file.");
}

uint64_t MapGenerator::getSeed() const {
    return seed;
}