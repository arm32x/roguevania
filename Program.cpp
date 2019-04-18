#include "Program.hpp"
using namespace RoguelikeMetroidvania;
using namespace sf;

RenderWindow Program::window(VideoMode::getDesktopMode(), "Roguelike Metroidvania (working title)", Style::Fullscreen);

void Program::main(int argc, char** argv) {
    
}

int main(int argc, char** argv) {
    Program::main(argc, argv);
    return 0;
}