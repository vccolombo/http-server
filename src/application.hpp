#pragma once

#include <boost/noncopyable.hpp>
#include <memory>

#include "using.hpp"

namespace httpserver
{

class Application : private boost::noncopyable
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
