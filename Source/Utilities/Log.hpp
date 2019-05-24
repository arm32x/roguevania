#pragma once

#include <iostream>

namespace RoguelikeMetroidvania {
namespace Utilities {

class Log {
        
    public:
        enum Level {
            Debug,
            Trace,
            Info,
            Warning,
            Error
        };
        
        const std::string mainTag;
        const Log::Level minimumLevel;
        Log(const std::string& mainTag, const Log::Level level);
        
        std::ostream& operator()(Log::Level level) const;
        std::ostream& operator()(Log::Level level, const char* tag) const;
        
        static std::ostream null;
        
    private:
        static struct NullStreamBuffer : public std::streambuf {
            int overflow(int c) { return c; }
        } nullStreamBuffer;
        
};

};
};