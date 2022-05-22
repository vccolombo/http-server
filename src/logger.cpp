#include "logger.hpp"

#include <chrono>
#include <cstring>
#include <ctime>

namespace httpserver::logging
{

Logger::Logger(std::string name, Level minimum_level, std::ostream& out)
    : name_(std::move(name)), minimum_level_(minimum_level), out_(out), null_out_(NullStream())
{
}

std::ostream& Logger::log(Level level)
{
    if (minimum_level_ > level)
    {
        return null_out_;
    }

    out_ << time_now() << " [" << level_string(level) << "] " << name_ << ": ";
    return out_;
}

std::ostream& Logger::debug()
{
    return log(Level::DEBUG);
}

std::ostream& Logger::info()
{
    return log(Level::INFO);
}

std::ostream& Logger::warn()
{
    return log(Level::WARN);
}

std::ostream& Logger::error()
{
    return log(Level::ERROR);
}

std::ostream& Logger::critical()
{
    return log(Level::CRITICAL);
}

// TODO: move this to somewhere else. It doesn't belong to this class
std::string Logger::time_now() const
{
    // https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
    auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    // https://stackoverflow.com/questions/9101590/fprintf-and-ctime-without-passing-n-from-ctime
    return strtok(std::ctime(&time), "\n");
}

}  // namespace httpserver::logging