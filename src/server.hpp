#pragma once

#include <boost/asio.hpp>
#include <memory>

#include "applicationfactory.hpp"
#include "logger.hpp"

namespace httpserver
{

using boost::asio::io_context;
using boost::asio::ip::tcp;

class TCPServer
{
   public:
    TCPServer(io_context& io_context, tcp::endpoint& endpoint, ApplicationFactory& app_factory);

   private:
    void accept();

    tcp::acceptor acceptor_;
    ApplicationFactory& app_factory_;
    logging::Logger logger_{logging::create_logger("TCPServer")};
};

}  // namespace httpserver
