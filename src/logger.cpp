#include "logger.hpp"

#include <utility>

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

std::string Logger::time_now() const
{
    return "<time placeholder>";
}

}  // namespace httpserver::logging