#pragma once

#include <string>

namespace httpserver
{

enum class Method
{
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    INVALID,
};

struct HeaderParams
{
    Method method;
    std::string path;
};

struct Request
{
    HeaderParams header;
    std::string body;
};

}  // namespace httpserver