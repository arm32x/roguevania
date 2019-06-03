#pragma once

#include "Exception.hpp"

namespace Roguevania {
namespace Exceptions {

////////////////////////////////////////////////////////////////////////////////
/// An exception used when file IO fails.
////////////////////////////////////////////////////////////////////////////////
class FileIOException : public Exception {
        
    public:
        ////////////////////////////////////////////////////////////////////////
        using Exception::Exception;
        
};

};
};