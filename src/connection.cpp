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
                auto data = get_data_from_buffer(bytes_transferred);
                auto keep_alive = app_.on_data(data, bytes_transferred);
                delete[] data;
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

uint8_t* Connection::get_data_from_buffer(std::size_t n_bytes)
{
    // https://www.boost.org/doc/libs/1_79_0/doc/html/boost_asio/reference/streambuf.html
    buffer_.commit(n_bytes);
    std::istream is(&buffer_);
    // https://codereview.stackexchange.com/a/28759
    auto data = new uint8_t[n_bytes];
    is.read(reinterpret_cast<char*>(data), n_bytes);

    return data;
}

}  // namespace httpserver