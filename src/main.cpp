#include <iostream>
#include <memory>

#include "server.hpp"
// fn callback(HTTPParams& params) {}
//

namespace httpserver
{
class HTTPServer final : public Application
{
   public:
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

    bool on_data(uint8_t* data, std::size_t length) override
    {
        // https://stackoverflow.com/questions/4508911/convert-uint8-t-to-stdstring-in-c
        std::string http_msg = std::string(data, data + length);
        std::cout << http_msg << std::endl;

        char response[] = "Hello world";
        writer_->write(reinterpret_cast<uint8_t*>(response), strlen(response));

        return false;
    }

    //    // register get
    //    void get(path, callback);
    //
    //    // register post
    //    void post();

   private:
};

class HTTPServerFactory final : public ApplicationFactory
{
   public:
    Application_ptr create() override
    {
        return std::make_unique<HTTPServer>();
    }

   private:
};
}  // namespace httpserver

int main(int argc, char* argv[])
{
    uint16_t port = std::atoi(argv[1]);

    httpserver::io_context io_context;
    httpserver::tcp::endpoint endpoint(httpserver::tcp::v4(), port);

    //    Router router;
    //    router.get("/", callback);
    httpserver::HTTPServerFactory http_server_factory;
    httpserver::TCPServer server(io_context, endpoint, http_server_factory);

    io_context.run();

    return 0;
}
