#pragma once

#include "FileIOException.hpp"

namespace RoguelikeMetroidvania {
namespace Exceptions {

////////////////////////////////////////////////////////////////////////////////
/// An exception used when a requested file cannot be found.
////////////////////////////////////////////////////////////////////////////////
class FileNotFoundException : public FileIOException {
        
    public:
        ////////////////////////////////////////////////////////////////////////
        using FileIOException::FileIOException;
        
};

};
};