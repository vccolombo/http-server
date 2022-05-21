#pragma once

namespace httpserver
{

class Application
{
   public:
    virtual ~Application() = default;

    // returns true if the connection should be kept open
    virtual bool on_data(uint8_t* data, std::size_t length) = 0;

   protected:
    Application() = default;
};

}  // namespace httpserver
