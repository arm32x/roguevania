#include "Exception.hpp"

using namespace Roguevania;
using namespace Roguevania::Exceptions;

Exception::Exception(const char* message)
    : message(message) {
    
}

const char* Exception::what() const noexcept {
    return message;
}