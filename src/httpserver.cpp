#include "httpserver.hpp"

namespace httpserver
{

bool HTTPServer::on_data(uint8_t* data, std::size_t length)
{
    // https://stackoverflow.com/questions/4508911/convert-uint8-t-to-stdstring-in-c
    std::string request_text = std::string(data, data + length);
    std::cout << request_text << std::endl;

    auto header_params = parse_header(request_text);
    std::cout << header_params.path << "\n";

    Request req = {.header = header_params, .body = ""};
    Response res;

    if (header_params.method == Method::GET)
    {
        auto callback = router_.get(header_params.path);
        if (callback)
        {
            callback(req, res);

            auto data = res.data();
            auto data_len = data.length();

            // TODO: create a class to wrap this buffer and allow any size
            char buf[1024 + data_len];
            snprintf(buf, sizeof(buf),
                "HTTP/1.1 %d OK\r\nContent-Length: %lu\r\nContent-Type: text/html\r\n\r\n%s",
                res.status(), data_len, data.c_str());
            writer_->write(reinterpret_cast<uint8_t*>(buf), strlen(buf));
        }
        else
        {
            char buf[4096];
            snprintf(buf, sizeof(buf), "HTTP/1.1 %d\r\nContent-Length: 0\r\n\r\n", 404);
            writer_->write(reinterpret_cast<uint8_t*>(buf), strlen(buf));
        }
    }
    else
    {
        // TODO: not implemented
    }

    return false;
}

struct HeaderParams HTTPServer::parse_header(const std::string& request_text)
{
    return HeaderParams{
        .method = parse_method(request_text),
        .path = parse_path(request_text),
    };
}

Method HTTPServer::parse_method(const std::string& request_text)
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

std::string HTTPServer::parse_path(const std::string& request_text)
{
    auto start_pos = request_text.find('/');
    auto end_pos = request_text.find(' ', start_pos);
    auto length = end_pos - start_pos;
    auto full_query = request_text.substr(start_pos, length);
    return full_query;
}

HTTPServerFactory::HTTPServerFactory(Router& router) : router_(router) {}

Application_ptr HTTPServerFactory::create()
{
    return std::make_unique<HTTPServer>(router_);
}

}  // namespace httpserver