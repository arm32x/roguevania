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
#include "Utilities/HSVtoRGB.hpp"

using namespace RoguelikeMetroidvania;
using namespace RoguelikeMetroidvania::Collision;
using namespace RoguelikeMetroidvania::Maps;
using namespace RoguelikeMetroidvania::Platform;
using namespace RoguelikeMetroidvania::Utilities;
using namespace sf;

#define WINDOWED_SCALE 2

#if WINDOWED_SCALE == 0
RenderWindow Program::window(VideoMode(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().width * (9.0 / 16.0)), "Roguelike Metroidvania (working title)", Style::Fullscreen);
#else
RenderWindow Program::window(VideoMode(640 * WINDOWED_SCALE, 360 * WINDOWED_SCALE), "Roguelike Metroidvania (working title)", Style::Titlebar | Style::Close);
#endif
View Program::gameView(FloatRect(0, 0, 640, 360));
Log Program::log("RogueMetroid", Log::Debug);

void Program::main(int argc, char** argv) {
    window.setVerticalSyncEnabled(true);
    window.setView(gameView);
    #ifdef _WIN32
    // Enable ANSI escape codes.
    SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), 0x0005);
    SetConsoleMode(GetStdHandle(STD_ERROR_HANDLE),  0x0005);
    #endif // _WIN32
    
    constexpr uint16_t width = 32, height = 16;
    
    MapGenerator gen;
    //gen.setSeed(0xD15EA5E);
    
    Program::log(Log::Trace, "MapGenerator") << "Generating map..." << std::endl;
    
    std::deque<Room> rooms = gen.generateRooms(width, height);
    std::deque<Section> sections = gen.generateSections(rooms, width, height, 32);
    
    Minimap minimap(width, height);
    for (Room& room : rooms) {
        minimap.setTileType(room.x, room.y, room.doors | room.openings << 4);
        if (room.section != nullptr) {
            minimap.setTileColor(room.x, room.y, room.section->color);
        } else {
            minimap.setTileColor(room.x, room.y, Color::Black);
        }
    }
    
    Program::log(Log::Level::Trace, "MapGenerator") << "Done!" << std::endl;
    
    Tilemap testTilemap(40, 23, 16, "Resources/Tilemaps/Default.png");
    testTilemap.setPosition(0.0f, -8.0f);
    testTilemap.loadFromFile("Resources/Tilemaps/Testing.tilmap");
    Color color;
    for (Room& room : rooms) {
        if (room.section != nullptr) {
            color = room.section->color;
            minimap.setPosition(room.x * -8 + 24, room.y * -8 + 24);
            break;
        }
    }
    for (uint16_t y = 0; y < 23; y++) {
        for (uint16_t x = 0; x < 40; x++) {
            testTilemap.setTileColor(x, y, color);
        }
    }
    
    CollisionMap testCollisionMap;
    testCollisionMap.loadFromFile("Resources/Tilemaps/Default.colmap");
    
    TilemapCollider testCollider(testTilemap, testCollisionMap);
    
    Texture entitiesTexture;
    entitiesTexture.loadFromFile("Resources/Spritesheets/Entities.png");
    Entity testEntity(entitiesTexture, IntRect(0, 0, 16, 32));
    testEntity.setPosition(32.0f, 64.0f);
    testEntity.setVelocity(0.0f, -4.0f);
    
    Clock frameClock;
    const Time optimalTime = seconds(1.0f) / 60.0f;
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
                        exit(EXIT_SUCCESS);
                        break;
                    case Event::KeyPressed:
                        switch (e.key.code) {
                            case Keyboard::Left:
                                minimap.move( 8,  0);
                                break;
                            case Keyboard::Down:
                                minimap.move( 0, -8);
                                break;
                            case Keyboard::Right:
                                minimap.move(-8,  0);
                                break;
                            case Keyboard::Up:
                                minimap.move( 0,  8);
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
                testEntity.event(e);
                #pragma GCC diagnostic pop
            }
        }
        
        Time frameTime = frameClock.restart();
        float delta = frameTime / optimalTime;
        int substeps = 1;
        while (delta >= 1.5f) {
            delta    /= 2;
            substeps *= 2;
        }
        if (substeps > 1) Program::log(Log::Warning, "GameLoop") << "Split " << (delta * substeps) << " delta into " << substeps << " substeps of " << delta << ".  Is the game overloaded?" << std::endl;
        for (; substeps > 0; substeps--) {
            testEntity.update(delta);
        }
        
        window.clear();
        window.draw(testTilemap);
        Vector2f testEntityOldPosition = testEntity.alignPosition();
        window.draw(testEntity);
        testEntity.setPosition(testEntityOldPosition);
        window.draw(minimap);
        window.display();
    }
}

int main(int argc, char** argv) {
    Program::main(argc, argv);
    return 0;
}