#pragma once

#include <string>

#include "logger.hpp"
#include "request.hpp"

namespace httpserver
{

static Method parse_method(const std::string& request_text)
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

static inline bool path_has_query(const std::string& path)
{
    return path.find('?') != std::string::npos;
}

static std::string get_path(const std::string& request_text)
{
    auto start_pos = request_text.find('/');
    auto end_pos = request_text.find(' ', start_pos);
    auto length = end_pos - start_pos;
    auto full_path = request_text.substr(start_pos, length);
    return full_path;
}

static std::string parse_path(const std::string& request_text)
{
    // TODO: what if it's malformed?
    auto full_path = get_path(request_text);
    if (path_has_query(full_path))
    {
        return full_path.substr(0, full_path.find('?'));
    }

    return full_path;
}

static std::string parse_query_key(const std::string& query)
{
    auto end_pos = query.find('=');
    if (end_pos == std::string::npos)
    {
        // TODO: throw?
        return "";
    }
    return query.substr(0, end_pos);
}

static std::string parse_query_value(const std::string& query)
{
    auto start_pos = query.find('=') + 1;
    if (start_pos == std::string::npos)
    {
        // TODO: throw?
        return "";
    }
    return query.substr(start_pos, query.length() - start_pos);
}

static std::unordered_map<std::string, std::string> parse_query(const std::string& request_text)
{
    static logging::Logger logger = logging::create_logger("parse_query");

    std::unordered_map<std::string, std::string> query;

    // TODO: what if it's malformed?
    auto full_path = get_path(request_text);
    if (!path_has_query(full_path))
    {
        return query;
    }

    auto start_pos = full_path.find('?') + 1;  // start from the first char after the '?'
    auto end_pos = full_path.find(' ');
    auto query_string = full_path.substr(start_pos, end_pos - start_pos);
    size_t pos;
    while ((pos = query_string.find('&')) != std::string::npos)
    {
        auto token = query_string.substr(0, pos);
        auto key = parse_query_key(token);
        auto value = parse_query_value(token);
        query_string.erase(0, pos + 1);

        query[key] = value;

        logger.debug() << key << ": " << value << std::endl;
    }
    // TODO: this being treated here is kinda ugly
    // There is one more query after the last '&'
    auto key = parse_query_key(query_string);
    auto value = parse_query_value(query_string);
    query[key] = value;
    logger.debug() << key << ": " << value << std::endl;

    return query;
}

inline struct HeaderParams parse_header(const std::string& request_text)
{
    return HeaderParams{
        .method = parse_method(request_text),
        .path = parse_path(request_text),
        .query = parse_query(request_text),
    };
}

}  // namespace httpserver
