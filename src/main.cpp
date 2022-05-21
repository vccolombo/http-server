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
    bool on_data(uint8_t* data, std::size_t length) override
    {
        // https://stackoverflow.com/questions/4508911/convert-uint8-t-to-stdstring-in-c
        std::string request_text = std::string(data, data + length);
        std::cout << request_text << std::endl;

        auto header_params = parse_header(request_text);
        std::cout << header_params.path << "\n";

        if (header_params.method == Method::GET)
        {
            char response[] = "Hello world";
            writer_->write(reinterpret_cast<uint8_t*>(response), strlen(response));
        }
        else
        {
            // TODO: not implemented
        }

        return false;
    }

    //    // register get
    //    void get(path, callback);
    //
    //    // register post
    //    void post();

   private:
    enum class Method
    {
        GET,
        HEAD,
        POST,
        PUT,
        DELETE,
        INVALID,
    };

    struct HeaderParams
    {
        Method method;
        std::string path;
    };

    struct HeaderParams parse_header(const std::string& request_text)
    {
        return HeaderParams{
            .method = parse_method(request_text),
            .path = parse_path(request_text),
        };
    }

    Method parse_method(const std::string& request_text)
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

    std::string parse_path(const std::string& request_text)
    {
        auto start_pos = request_text.find('/');
        auto end_pos = request_text.find(' ', start_pos);
        auto length = end_pos - start_pos;
        auto full_query = request_text.substr(start_pos, length);
        return full_query;
    }
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
