#pragma once

#include <cstdint>

namespace httpserver
{

class Writer
{
   public:
    virtual ~Writer() = default;

    virtual void write(uint8_t* data, std::size_t length) = 0;

   protected:
    Writer() = default;
};

}  // namespace httpserver