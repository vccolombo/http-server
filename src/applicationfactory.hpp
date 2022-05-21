#pragma once

#include "application.hpp"

namespace httpserver
{

class ApplicationFactory
{
   public:
    virtual Application_ptr create() = 0;

   protected:
    ApplicationFactory() = default;
    ~ApplicationFactory() = default;
};

}  // namespace httpserver
