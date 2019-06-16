#include "Program.hpp"

#include <bitset>
#include <cmath>
#include <deque>
#include <functional>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

#include <SFML/System/Sleep.hpp>
#include <SFML/Window/Event.hpp>

#include "Collision/CollisionMap.hpp"
#include "Collision/TilemapCollider.hpp"
#include "Maps/Minimap.hpp"
#include "Maps/MapGenerator.hpp"
#include "Maps/Room.hpp"
#include "Maps/Section.hpp"
#include "Maps/Tilemap.hpp"
#include "Platform/Entity.hpp"
#include "Platform/Player.hpp"
#include "Utilities/HSVtoRGB.hpp"

using namespace Roguevania;
using namespace Roguevania::Collision;
using namespace Roguevania::Maps;
using namespace Roguevania::Platform;
using namespace Roguevania::Utilities;
using namespace sf;

#define WINDOWED_SCALE 2
#define CAMERA_MODE 1

#if WINDOWED_SCALE == 0
RenderWindow Program::window(VideoMode(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().width * (9.0 / 16.0)), "Roguevania (working title)", Style::Fullscreen);
#else
RenderWindow Program::window(VideoMode(640 * WINDOWED_SCALE, 360 * WINDOWED_SCALE), "Roguevania (working title)", Style::Titlebar | Style::Close);
#endif
Camera Program::camera;
Log Program::log("Roguevania", Log::Debug);

void Program::main(int argc, char** argv) {
    window.setVerticalSyncEnabled(true);
    window.setView(camera.view);
    #ifdef _WIN32
    // Enable ANSI escape codes.
    SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), 0x0005);
    SetConsoleMode(GetStdHandle(STD_ERROR_HANDLE),  0x0005);
    #endif // _WIN32
    
    bool pause = false;
    
    constexpr uint16_t width = 32, height = 16;
    
    MapGenerator gen;
    
    Program::log(Log::Trace, "MapGenerator") << "Generating map..." << std::endl;
    Program::log(Log::Info, "MapGenerator") << "Using seed " << gen.getSeed() << "u." << std::endl;
    
    std::deque<Room> rooms = gen.generateRooms(width, height);
    std::deque<Section> sections = gen.generateSections(rooms, width, height, 32);
    
    Program::log(Log::Trace, "MapGenerator") << "Done!" << std::endl;
    
    Minimap minimap(width, height);
    for (Room& room : rooms) {
        minimap.setTileType(room.x, room.y, room.doors | room.openings << 4);
        if (room.section != nullptr) {
            minimap.setTileColor(room.x, room.y, room.section->color);
        } else {
            minimap.setTileColor(room.x, room.y, Color::Black);
        }
    }
    
    CollisionMap clmp;
    clmp.loadFromFile("Resources/Tilemaps/Default.clmp");
    
    for (Room& room : rooms) {
        if (room.section == nullptr) continue;
        gen.generateRoomLayout(room);
        room.tilemap->collider.emplace(*room.tilemap, clmp);
        room.tilemap->setPosition(room.x * 640, room.y * 368);
        for (uint16_t y = 0; y < 23; y++) {
            for (uint16_t x = 0; x < 40; x++) {
                room.tilemap->setTileColor(x, y, room.section->color);
            }
        }
    }
    
    std::deque<Room*> startingRoomCandidates;
    for (Room& room : rooms) {
        if (room.section == nullptr) continue;
        if (room.section->id != 'A') continue;
        if (room.doors    != 0b0101) continue;
        if (room.openings != 0b0000) continue;
        startingRoomCandidates.push_front(&room);
    }
    Room& startingRoom = *gen.getRandom().pick(startingRoomCandidates);
    
    Texture entitiesTexture;
    entitiesTexture.loadFromFile("Resources/Spritesheets/Entities.png");
    Player player(entitiesTexture, IntRect(16, 2, 16, 30));
    player.setPosition(startingRoom.x * 640.0f + (360.0f - 8.0f), startingRoom.y * 368.0f + (188.0f - 8.0f));
    
    Clock clock;
    Time accumulator;
    const Time optimalTime = seconds(1.0f / 60.0f);
    while (window.isOpen()) {
        {
            Event e;
            while (window.pollEvent(e)) {
                #pragma GCC diagnostic push
                #pragma GCC diagnostic ignored "-Wswitch"
                switch (e.type) {
                    case Event::Closed:
                        Program::log(Log::Info) << "Window closed, shutting down." << std::endl;
                        window.close();
                        break;
                    case Event::KeyPressed:
                        switch (e.key.code) {
                            case Keyboard::Tilde:
                                Program::log(Log::Info) << "Program will now pause on exit." << std::endl;
                                pause = true;
                                break;
                        }
                        break;
                    case Event::MouseButtonPressed:
                        switch (e.mouseButton.button) {
                            case Mouse::Left:
                                Program::log(Log::Info) << "Coordinate location requested:  (" << window.mapPixelToCoords(Mouse::getPosition(window)).x << ", " << window.mapPixelToCoords(Mouse::getPosition(window)).y << ")." << std::endl;
                                break;
                            case Mouse::Right:
                                break;
                        }
                        break;
                }
                player.event(e);
                #pragma GCC diagnostic pop
            }
        }
        
        while (accumulator >= optimalTime) {
            player.update(1.0f);
            #if CAMERA_MODE == 0
            camera.update(1.0f, player.getPosition() + Vector2f(player.getTextureRect().width / 2, player.getTextureRect().height / 2) /* + Vector2f(Mouse::getPosition(window) / WINDOWED_SCALE - Vector2i(320, 180)) / 4.0f */);
            #elif CAMERA_MODE == 1
            camera.update(1.0f, Vector2f(std::floor((player.getPosition().x + player.getTextureRect().width / 2) / 640.0f) * 640.0f + 320.0f, std::floor((player.getPosition().y + player.getTextureRect().height - 8) / 368.0f) * 368.0f + 188.0f));
            #endif
            minimap.setPosition(std::floor((camera.getFocalPoint().x) / 640.0f) * -8 + 24, std::floor((camera.getFocalPoint().y) / 368.0f) * -8 + 24);
            accumulator -= optimalTime;
        }
        
        {
            Vector2f cameraOldCenter = camera.alignCenter();
            window.setView(camera.view);
            camera.view.setCenter(cameraOldCenter);
        }
        window.clear();
        for (Room& room : rooms) {
            if (room.tilemap) window.draw(*room.tilemap);
        }
        {
            Vector2f playerOldPosition = player.alignPosition();
            window.draw(player);
            player.setPosition(playerOldPosition);
        }
        window.draw(minimap);
        window.display();
        
        accumulator += clock.restart();
    }
    if (pause) system("pause");
}

int main(int argc, char** argv) {
    try {
        Program::main(argc, argv);
    } catch (...) {
        system("pause");
        throw;
    }
    return 0;
}