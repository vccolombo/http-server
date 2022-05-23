#include "server.hpp"

#include <boost/asio.hpp>
#include <memory>

#include "application.hpp"
#include "applicationfactory.hpp"
#include "connection.hpp"

namespace httpserver
{

TCPServer::TCPServer(
    io_context& io_context, tcp::endpoint& endpoint, ApplicationFactory& app_factory)
    : acceptor_(io_context, endpoint), app_factory_(app_factory)
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
                logger_.info() << "New connection from "
                               << socket.remote_endpoint().address().to_string() << ":"
                               << socket.remote_endpoint().port() << std::endl;
                auto app = app_factory_.create();
                std::make_shared<Connection>(std::move(socket), std::move(app))->accept();
            }
            else
            {
                logger_.error() << "async_accept " << ec.message() << std::endl;
            }

            accept();
        });
}

}  // namespace httpserver