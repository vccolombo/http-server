#pragma once

#include <string>
#include <unordered_map>

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
    std::unordered_map<std::string, std::string> query;
};

struct Request
{
    HeaderParams header;
    std::string body;
};

}  // namespace httpserver