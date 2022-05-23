#pragma once

#include "logger.hpp"
#include "using.hpp"

namespace httpserver
{

class ApplicationFactory;

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
