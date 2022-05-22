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

HTTPServerFactory::HTTPServerFactory(Router& router) : router_(router) {}

Application_ptr HTTPServerFactory::create()
{
    return std::make_unique<HTTPServer>(router_);
}

}  // namespace httpserver