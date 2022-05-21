#pragma once

#include <memory>

#include "writer.hpp"

namespace httpserver
{

class Application;
using Application_ptr = std::unique_ptr<Application>;

class Application
{
   public:
    virtual ~Application() = default;

    inline void register_writer(Writer_ptr writer)
    {
        writer_ = writer;
    }

    // returns true if the connection should be kept open
    virtual bool on_data(uint8_t* data, std::size_t length) = 0;

   protected:
    Application() = default;

    Writer_ptr writer_ = nullptr;
};

}  // namespace httpserver
