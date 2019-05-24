#include "Log.hpp"

using namespace RoguelikeMetroidvania;
using namespace Utilities;

Log::Log(const std::string& mainTag, const Log::Level minimumLevel)
    : mainTag(mainTag), minimumLevel(minimumLevel) {
    
}

std::ostream& Log::operator()(Log::Level level) const {
    if (level < minimumLevel) return Log::null;
    const char* prefix;
    switch (level) {
        case Log::Level::Debug:
            prefix = "\033[90m  Debug\033[00m";
            break;
        case Log::Level::Trace:
            prefix = "\033[97m  Trace\033[00m";
            break;
        case Log::Level::Info:
            prefix = "\033[94m   Info\033[00m";
            break;
        case Log::Level::Warning:
            prefix = "\033[93mWarning\033[00m";
            break;
        case Log::Level::Error:
            prefix = "\033[31m  Error\033[00m";
            break;
    }
    return (level >= Log::Level::Error ? std::cerr : std::cout) << prefix << " | " << mainTag << " | ";
}

std::ostream& Log::operator()(Log::Level level, const char* tag) const {
    if (level < minimumLevel) return Log::null;
    const char* prefix;
    switch (level) {
        case Log::Level::Debug:
            prefix = "\033[90m  Debug\033[00m";
            break;
        case Log::Level::Trace:
            prefix = "\033[00m  Trace\033[00m";
            break;
        case Log::Level::Info:
            prefix = "\033[94m   Info\033[00m";
            break;
        case Log::Level::Warning:
            prefix = "\033[93mWarning\033[00m";
            break;
        case Log::Level::Error:
            prefix = "\033[31m  Error\033[00m";
            break;
    }
    return (level >= Log::Level::Error ? std::cerr : std::cout) << prefix << " | " << mainTag << " - " << tag << " | ";
}

Log::NullStreamBuffer Log::nullStreamBuffer;
std::ostream Log::null(&Log::nullStreamBuffer);