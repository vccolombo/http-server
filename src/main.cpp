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

void callback_examplecom(httpserver::Request& req, httpserver::Response& res)
{
    static httpserver::logging::Logger logger{
        httpserver::logging::create_logger("callback_examplecom")};
    try
    {
        res.set_status(200);
        // supposed the examples folder is in the same directory as the binary is called from
        res.send_file("./examples/example.html");
    }
    catch (std::exception& e)
    {
        logger.error() << e.what() << std::endl;
        res.set_status(500);
    }
}

void callback_sample1(httpserver::Request& req, httpserver::Response& res)
{
    static httpserver::logging::Logger logger{
        httpserver::logging::create_logger("callback_sample1")};
    try
    {
        res.set_status(200);
        // supposed the examples folder is in the same directory as the binary is called from
        res.send_file("./examples/sample1.html");
    }
    catch (std::exception& e)
    {
        logger.error() << e.what() << std::endl;
        res.set_status(500);
    }
}

int main(int argc, char* argv[])
{
    static httpserver::logging::Logger logger{httpserver::logging::create_logger("main")};

    uint16_t port = std::atoi(argv[1]);
    logger.info() << "Starting server on port " << port << std::endl;

    httpserver::io_context io_context;
    httpserver::tcp::endpoint endpoint(httpserver::tcp::v4(), port);

    httpserver::Router router;
    router.register_get("/", callback_index);
    router.register_get("/example.html", callback_examplecom);
    router.register_get("/sample1.html", callback_sample1);
    httpserver::HTTPServerFactory http_server_factory(router);
    httpserver::TCPServer server(io_context, endpoint, http_server_factory);

    io_context.run();

    return 0;
}
