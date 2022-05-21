#include "server.hpp"

#include <boost/asio.hpp>
#include <memory>
#include <iostream>

#include "connection.hpp"

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
            if (!ec)
            {
                std::cout << "New connection from: " << socket.remote_endpoint().address().to_string()
                          << ":" << socket.remote_endpoint().port() << std::endl;
                std::make_shared<Connection>(std::move(socket), app_)->accept();
            }
            else {
                std::cout << "async_accept ERROR " << ec.message() << std::endl;
            }

            accept();
        });
}

}  // namespace httpserver