#include "Program.hpp"

#include <bitset>
#include <cmath>
#include <deque>
#include <functional>

#include <SFML/System/Sleep.hpp>
#include <SFML/Window/Event.hpp>

#include "Maps/Minimap.hpp"
#include "Maps/MapGenerator.hpp"
#include "Maps/Room.hpp"
#include "Maps/Section.hpp"
#include "Platform/Entity.hpp"
#include "Utilities/HSVtoRGB.hpp"

using namespace RoguelikeMetroidvania;
using namespace RoguelikeMetroidvania::Maps;
using namespace RoguelikeMetroidvania::Platform;
using namespace RoguelikeMetroidvania::Utilities;
using namespace sf;

RenderWindow Program::window(VideoMode(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().width * (9.0 / 16.0)), "Roguelike Metroidvania (working title)", Style::Fullscreen);
View Program::gameView(FloatRect(0, 0, 640, 360));

void Program::main(int argc, char** argv) {
    window.setVerticalSyncEnabled(true);
    window.setView(gameView);
    
    constexpr uint16_t width = 32, height = 16;
    
    MapGenerator gen;
    gen.setSeed(0xD15EA5E);
    
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
    
    Tilemap testTilemap(40, 23, 16, "Resources/Tilemaps/Default.png");
    testTilemap.setPosition(0.0f, -8.0f);
    testTilemap.setTileType(1, 19, 0x01);
    testTilemap.setTileType(2, 19, 0x02);
    testTilemap.setTileType(3, 19, 0x03);
    testTilemap.setTileType(1, 20, 0x11);
    testTilemap.setTileType(2, 20, 0x12);
    testTilemap.setTileType(3, 20, 0x13);
    testTilemap.setTileType(1, 21, 0x21);
    testTilemap.setTileType(2, 21, 0x22);
    testTilemap.setTileType(3, 21, 0x23);
    
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
                        window.close();
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
                }
                #pragma GCC diagnostic pop
            }
        }
        
        Time frameTime = frameClock.restart();
        float delta = frameTime / optimalTime;
        std::cout << delta << std::endl;
        testEntity.update(delta);
        
        window.clear();
        window.draw(testTilemap);
        window.draw(testEntity);
        window.draw(minimap);
        window.display();
    }
}

int main(int argc, char** argv) {
    Program::main(argc, argv);
    return 0;
}