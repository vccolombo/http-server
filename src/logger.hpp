#pragma once

#include <boost/noncopyable.hpp>
#include <cassert>
#include <iostream>

namespace httpserver::logging
{

// https://stackoverflow.com/questions/11826554/standard-no-op-output-stream
class NullBuffer : public std::streambuf
{
   public:
    int overflow(int c) override
    {
        return c;
    }
};

class NullStream : public std::ostream
{
   public:
    NullStream() : std::ostream(&null_buffer_) {}

   private:
    NullBuffer null_buffer_;
};

enum class Level
{
    DEBUG,
    INFO,
    WARN,
    ERROR,
    CRITICAL
};

inline std::string level_string(Level level)
{
    switch (level)
    {
        case Level::DEBUG:
            return "DEBUG";
        case Level::INFO:
            return "INFO";
        case Level::WARN:
            return "WARN";
        case Level::ERROR:
            return "ERROR";
        case Level::CRITICAL:
            return "CRITICAL";
        default:
            assert(false && "Invalid logging level value");
    }
}

class Logger : private boost::noncopyable
{
   public:
    Logger(std::string  name, Level minimum_level, std::ostream& out);

    [[nodiscard]] std::ostream& log(Level level);
    [[nodiscard]] std::ostream& debug();
    [[nodiscard]] std::ostream& info();
    [[nodiscard]] std::ostream& warn();
    [[nodiscard]] std::ostream& error();
    [[nodiscard]] std::ostream& critical();

   private:
    [[nodiscard]] std::string time_now() const;

    std::string name_;
    Level minimum_level_;
    std::ostream& out_;
    NullStream null_out_;
};

inline Logger create_logger(
    const std::string& name, Level level = Level::DEBUG, std::ostream& out = std::cerr)
{
    return Logger(name, level, out);
}

}  // namespace httpserver::logging
