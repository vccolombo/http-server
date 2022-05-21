#include "server.hpp"

// fn callback(HTTPParams& params) {}
//
class HTTPServer final : public httpserver::Application
{
   public:
    ~HTTPServer() override = default;
//    void on_data(NetworkMessage& msg)
//    {
//        parse
//
//            if GET : find path in get map;
//        if POST:
//             find
//         path
//         in
//         POST map;
//        else:
//         not implemented
//    };

//    // register get
//    void get(path, callback);
//
//    // register post
//    void post();

   private:
};

int main(int argc, char* argv[])
{
    uint16_t port = std::atoi(argv[1]);

    httpserver::io_context io_context;
    httpserver::tcp::endpoint endpoint(httpserver::tcp::v4(), port);

    HTTPServer http_server;
    httpserver::TCPServer server(io_context, endpoint, http_server);

//    http_server.get("/", callback);

    io_context.run();

    return 0;
}
