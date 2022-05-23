#pragma once

#include <cstdint>
#include <cstring>
#include <string>

#include "application.hpp"
#include "applicationfactory.hpp"
#include "logger.hpp"

namespace httpserver
{

class Router;
class Request;
class Response;

class HTTPServer final : public Application
{
   public:
    explicit HTTPServer(Router& router);

    bool on_data(uint8_t* data, std::size_t length) override;

    [[nodiscard]] Response do_get(const Request& req) const;

   private:
    Router& router_;
    logging::Logger logger_;
};

class HTTPServerFactory final : public ApplicationFactory
{
   public:
    explicit HTTPServerFactory(Router& router);

    Application_ptr create() override;

   private:
    Router& router_;
};

}  // namespace httpserver
