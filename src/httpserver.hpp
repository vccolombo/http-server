#pragma once

#include <cstdint>
#include <cstring>
#include <string>

#include "application.hpp"
#include "applicationfactory.hpp"
#include "httpparse.hpp"
#include "logger.hpp"
#include "request.hpp"
#include "response.hpp"
#include "router.hpp"

namespace httpserver
{

class HTTPServer final : public Application
{
   public:
    explicit HTTPServer(Router& router) : router_(router) {}

    bool on_data(uint8_t* data, std::size_t length) override;

    Response do_get(Request& req) const;

   private:
    Router& router_;
    logging::Logger logger_{logging::create_logger("HTTPServer")};
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
