#pragma once

#include <SFML/Graphics.hpp>

namespace RoguelikeMetroidvania {
using namespace sf;

////////////////////////////////////////////////////////////////////////////////
/// A class to contain the entry point of the program, similar to C# or Java.
////////////////////////////////////////////////////////////////////////////////
class Program final {
    
    public:
        ////////////////////////////////////////////////////////////////////////
        /// The SFML window that the program will be rendered in.
        ////////////////////////////////////////////////////////////////////////
        static RenderWindow window;
        
        ////////////////////////////////////////////////////////////////////////
        /// The main program entry point.
        ///
        /// @param argc The number of arguments in ‘argv’.
        /// @param argv A C array of C strings containing program arguments.
        /// 
        /// @note
        ///   - This is not the real entry point, instead this method is called 
        ///     from the real entry point in ‘main.cpp’.
        ////////////////////////////////////////////////////////////////////////
        static void main(int argc, char** argv);
        
    private:
        ////////////////////////////////////////////////////////////////////////
        /// The default constructor of this class is deleted so that instances
        /// of this class cannot be created.
        ////////////////////////////////////////////////////////////////////////
        Program() = delete;
    
};

};