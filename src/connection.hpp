#pragma once

#include <boost/asio.hpp>
#include <memory>

#include "application.hpp"

namespace httpserver
{

using boost::asio::ip::tcp;

class Connection : public std::enable_shared_from_this<Connection>
{
   public:
    Connection(tcp::socket socket, Application& app);

    void accept();

   private:
    void read();

    tcp::socket socket_;
    boost::asio::streambuf buffer_;
    Application& app_;
};

}  // namespace httpserver
