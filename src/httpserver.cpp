#include "httpserver.hpp"

namespace httpserver
{

bool HTTPServer::on_data(uint8_t* data, std::size_t length)
{
    // https://stackoverflow.com/questions/4508911/convert-uint8-t-to-stdstring-in-c
    std::string request_text = std::string(data, data + length);
    logger_.debug() << request_text << std::endl;

    auto header_params = parse_header(request_text);

    Request req = {.header = header_params, .body = ""};
    Response res;

    if (header_params.method == Method::GET)
    {
        auto callback = router_.get(header_params.path);
        if (callback)
        {
            callback(req, res);

            auto response_body = res.data();
            auto response_body_len = response_body.length();

            // TODO: create a class to wrap this buffer and allow any size
            char buf[1024 + response_body_len];
            snprintf(buf, sizeof(buf),
                "HTTP/1.1 %d OK\r\nContent-Length: %lu\r\nContent-Type: text/html\r\n\r\n%s",
                res.status(), response_body_len, response_body.c_str());
            writer_->write(reinterpret_cast<uint8_t*>(buf), sizeof(buf));
        }
        else
        {
            logger_.debug() << header_params.path + " not registered" << std::endl;
            char buf[64];
            snprintf(buf, sizeof(buf), "HTTP/1.1 %d\r\nContent-Length: 0\r\n\r\n", 404);
            writer_->write(reinterpret_cast<uint8_t*>(buf), sizeof(buf));
        }
    }
    else
    {
        // TODO: not implemented
        char buf[64];
        snprintf(buf, sizeof(buf), "HTTP/1.1 %d\r\nContent-Length: 0\r\n\r\n", 501);
        writer_->write(reinterpret_cast<uint8_t*>(buf), sizeof(buf));
    }

    return false;
}

HTTPServerFactory::HTTPServerFactory(Router& router) : router_(router) {}

Application_ptr HTTPServerFactory::create()
{
    return std::make_unique<HTTPServer>(router_);
}

}  // namespace httpserver