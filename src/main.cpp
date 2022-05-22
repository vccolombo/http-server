#include <iostream>
#include <map>
#include <memory>
#include <utility>

#include "server.hpp"

namespace httpserver
{

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

struct Request
{
    HeaderParams header;
    std::string body;
};

class Response
{
   public:
    using status_code_t = uint16_t;

    Response() : status_code_(200) {}

    void set_status(status_code_t status_code)
    {
        status_code_ = status_code;
    }

    void send(const std::string& data)
    {
        data_ = data;
    }

    void send_file(const std::string& path){
        // TODO
    };

    [[nodiscard]] status_code_t status() const
    {
        return status_code_;
    }

    [[nodiscard]] std::string data() const
    {
        return data_;
    }

   private:
    status_code_t status_code_;
    std::string data_;
};

class Router
{
   public:
    void register_get(
        const std::string& path, const std::function<void(Request&, Response&)>& callback)
    {
        getters_[path] = callback;
    }

    std::function<void(Request&, Response&)> get(const std::string& path)
    {
        // I'm presuming this returns an empty function if it was not registered before. Need to
        // test but it's still a bad idea, as it'll increase the size of the map. So:
        // TODO: check if key exists
        return getters_[path];
    }

   private:
    std::map<std::string, std::function<void(Request&, Response&)>> getters_;
};

class HTTPServer final : public Application
{
   public:
    explicit HTTPServer(Router& router) : router_(router) {}

    bool on_data(uint8_t* data, std::size_t length) override
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

   private:
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

    Router& router_;
};

class HTTPServerFactory final : public ApplicationFactory
{
   public:
    explicit HTTPServerFactory(Router& router) : router_(router) {}

    Application_ptr create() override
    {
        return std::make_unique<HTTPServer>(router_);
    }

   private:
    Router& router_;
};
}  // namespace httpserver

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
