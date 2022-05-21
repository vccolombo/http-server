#pragma once

namespace httpserver
{

class Application
{
   public:
    virtual ~Application() = default;

    virtual bool on_data() = 0;

   protected:
    Application() = default;
};

}  // namespace httpserver
