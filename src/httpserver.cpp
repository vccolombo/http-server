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

    if (header_params.method == Method::GET)
    {
        auto res = do_get(req);
        auto response_body = res.data();
        auto response_body_len = response_body.length();

        // TODO: create a class to wrap this buffer and allow any size
        char* buf = new char[4096 + response_body_len];
        snprintf(buf, 4096 + response_body_len, "HTTP/1.1 %d\r\nContent-Length: %lu\r\n\r\n%s",
            res.status(), response_body_len, response_body.c_str());
        logger_.debug() << buf << std::endl;
        writer_->write(reinterpret_cast<uint8_t*>(buf), strlen(buf));
        delete[] buf;
    }
    if (header_params.method == Method::HEAD)
    {
        auto res = do_get(req);
        auto response_body = res.data();
        auto response_body_len = response_body.length();

        // TODO: create a class to wrap this buffer and allow any size
        char buf[4096];
        snprintf(buf, sizeof(buf), "HTTP/1.1 %d\r\nContent-Length: %lu\r\n\r\n", res.status(),
            response_body_len);
        logger_.debug() << buf << std::endl;
        writer_->write(reinterpret_cast<uint8_t*>(buf), strlen(buf));
    }
    else
    {
        // TODO: not implemented
        char buf[64];
        snprintf(buf, sizeof(buf), "HTTP/1.1 %d\r\nContent-Length: 0\r\n\r\n", 501);
        writer_->write(reinterpret_cast<uint8_t*>(buf), strlen(buf));
    }

    return false;
}

Response HTTPServer::do_get(const Request& req) const
{
    Response res;
    auto callback = router_.get(req.header.path);
    if (callback)
    {
        callback(req, res);
    }
    else
    {
        res.set_status(404);
    }

    return res;
}

HTTPServerFactory::HTTPServerFactory(Router& router) : router_(router) {}

Application_ptr HTTPServerFactory::create()
{
    return std::make_unique<HTTPServer>(router_);
}

}  // namespace httpserver