#pragma once

namespace httpserver
{

class Application
{
   public:
    virtual ~Application() = default;

   protected:
    Application() = default;

//    void on_data();
};

}  // namespace httpserver
