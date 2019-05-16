#include "Exception.hpp"

using namespace RoguelikeMetroidvania;
using namespace RoguelikeMetroidvania::Exceptions;

Exception::Exception(const char* message)
    : message(message) {
    
}

const char* Exception::what() const noexcept {
    return message;
}