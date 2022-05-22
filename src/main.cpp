#include "httpserver.hpp"
#include "request.hpp"
#include "response.hpp"
#include "router.hpp"
#include "server.hpp"

void callback_index(httpserver::Request& req, httpserver::Response& res)
{
    res.set_status(200);
    res.send("Hello World!!\r\nThis is a sample app for my http server\r\n;)");
}

int main(int argc, char* argv[])
{
    uint16_t port = std::atoi(argv[1]);

    httpserver::io_context io_context;
    httpserver::tcp::endpoint endpoint(httpserver::tcp::v4(), port);

    httpserver::Router router;
    router.register_get("/", callback_index);
    httpserver::HTTPServerFactory http_server_factory(router);
    httpserver::TCPServer server(io_context, endpoint, http_server_factory);

    io_context.run();

    return 0;
}
