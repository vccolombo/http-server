#pragma once

#include <boost/asio.hpp>

#include "application.hpp"

namespace httpserver
{

using boost::asio::io_context;
using boost::asio::ip::tcp;

class TCPServer
{
   public:
    TCPServer(io_context& io_context, tcp::endpoint& endpoint, Application& app);

   private:
    tcp::acceptor acceptor_;
    Application& app_;
};

}  // namespace httpserver
