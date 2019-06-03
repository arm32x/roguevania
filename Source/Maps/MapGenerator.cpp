#include "MapGenerator.hpp"

#include <fstream>
#include <iostream>

#include "../Exceptions/FileNotFoundException.hpp"
#include "../Maps/Room.hpp"
#include "../Maps/Section.hpp"
#include "../Utilities/HSVtoRGB.hpp"
#include "../Program.hpp"

using namespace Roguevania;
using namespace Roguevania::Maps;

MapGenerator::MapGenerator(uint64_t seed)
    : random(seed) {
    
}

MapGenerator::MapGenerator()
    : random() {
    
}

void MapGenerator::setSeed(uint64_t seed) {
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
        sections.emplace_front(sectionID, Utilities::HSVtoRGB(hue, 0.75, 0.75));
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
    for (uint16_t y = 0; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            {
                // Replace “ring” room formations with large square rooms.
                if ((rooms[(x    ) + width * (y    )].openings) != 0b0000) continue;
                if ((rooms[(x    ) + width * (y    )].doors & 0b0110) != 0b0110) continue;
                if ((rooms[(x + 1) + width * (y    )].doors & 0b0011) != 0b0011) continue;
                if ((rooms[(x    ) + width * (y + 1)].doors & 0b1100) != 0b1100) continue;
                if ((rooms[(x + 1) + width * (y + 1)].doors & 0b1001) != 0b1001) continue;
                rooms[(x    ) + width * (y    )].openings |= 0b0110;
                rooms[(x + 1) + width * (y    )].openings |= 0b0011;
                rooms[(x    ) + width * (y + 1)].openings |= 0b1100;
                rooms[(x + 1) + width * (y + 1)].openings |= 0b1001;
            }
        }
    }
}

randutils::mt19937_rng& MapGenerator::getRandom() {
    return random;
}

Tilemap* MapGenerator::generateRoomLayoutFromFile(Room& room, const char* filename) {
    std::ifstream stream;
    stream.open(filename, std::ios::in);
    if (stream.fail()) {
        Program::log(Log::Error, "MapGenerator") << "Could not open room template file '" << filename << "'." << std::endl;
        throw Exceptions::FileNotFoundException("Could not open room template file.");
    }
    return generateRoomLayoutFromStream(room, stream);
}

Tilemap* MapGenerator::generateRoomLayoutFromStream(Room& room, std::istream& stream) {
    
}