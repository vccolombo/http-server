#pragma once

#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>

#include "application.hpp"
#include "applicationfactory.hpp"
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

   private:
    struct HeaderParams parse_header(const std::string& request_text);
    Method parse_method(const std::string& request_text);
    std::string parse_path(const std::string& request_text);

    Router& router_;
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
