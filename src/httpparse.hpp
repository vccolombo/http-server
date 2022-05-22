#pragma once

#include <string>

#include "request.hpp"

namespace httpserver
{

inline Method parse_method(const std::string& request_text)
{
    if (request_text.starts_with("GET"))
    {
        return Method::GET;
    }
    else if (request_text.starts_with("HEAD"))
    {
        return Method::HEAD;
    }
    else if (request_text.starts_with("POST"))
    {
        return Method::POST;
    }
    else if (request_text.starts_with("PUT"))
    {
        return Method::PUT;
    }
    else if (request_text.starts_with("DELETE"))
    {
        return Method::DELETE;
    }
    return Method::INVALID;
}

inline std::string parse_path(const std::string& request_text)
{
    auto start_pos = request_text.find('/');
    auto end_pos = request_text.find(' ', start_pos);
    auto length = end_pos - start_pos;
    auto full_query = request_text.substr(start_pos, length);
    return full_query;
}

inline struct HeaderParams parse_header(const std::string& request_text)
{
    return HeaderParams{
        .method = parse_method(request_text),
        .path = parse_path(request_text),
    };
}

}  // namespace httpserver
