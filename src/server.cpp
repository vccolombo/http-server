#include "server.hpp"

#include <iostream>

namespace httpserver
{

TCPServer::TCPServer(io_context& io_context, tcp::endpoint& endpoint, Application& app)
    : acceptor_(io_context, endpoint), app_(app)
{
    accept();
}
void TCPServer::accept()
{
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
            std::cout << "New connection from: " << socket.remote_endpoint().address().to_string()
                      << ":" << socket.remote_endpoint().port() << std::endl;
            if (!ec)
            {
                read(std::move(socket));
            }

            accept();
        });
}

void TCPServer::read(tcp::socket socket) {}

}  // namespace httpserver