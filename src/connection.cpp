#include "connection.hpp"

#include <iostream>

namespace httpserver
{
Connection::Connection(tcp::socket socket, Application& app) : socket_(std::move(socket)), app_(app)
{
}

void Connection::accept()
{
    read();
}

void Connection::read()
{
    auto self(shared_from_this());
    boost::asio::async_read_until(socket_, buffer_, "\r\n\r\n",
        [this, self](const boost::system::error_code& ec, std::size_t bytes_transferred)
        {
            if (!ec)
            {
                //                std::string line;
                //                std::istream is(buffer_);
                //                std::getline(is, line);
                //                buffer_.consume(buffer_.size());

                auto keep_alive = app_.on_data();
                if (keep_alive)
                {
                    read();
                }
            }
            else
            {
                std::cout << "async_read_until ERROR " << ec.message() << std::endl;
            }
        });
}
}  // namespace httpserver