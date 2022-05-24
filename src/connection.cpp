#include "connection.hpp"

namespace httpserver
{

Connection::Connection(tcp::socket socket, Application_ptr app)
    : socket_(std::move(socket)), app_(std::move(app))
{
    // for some reason I just can't understand, changing this to shared_from_this()
    // (and for course changing Writer_ptr to shared_ptr)
    // cause some weird bug where the browser waits forever and a request for favicon never arrives,
    // so I'll just use a pointer here, it should be guaranteed that the Connection object outlives
    // the app
    app_->register_writer(this);
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
                auto keep_alive = app_->on_data(data, bytes_transferred);
                delete[] data;
                if (keep_alive)
                {
                    read();
                }
            }
            else
            {
                logger_.error() << "async_read_until " << ec.message() << std::endl;
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
    is.read(reinterpret_cast<char*>(data), static_cast<long>(n_bytes));

    return data;
}

void Connection::write(uint8_t* data, std::size_t length)
{
    boost::asio::async_write(socket_, boost::asio::buffer(data, length),
        [this](const boost::system::error_code& ec, std::size_t /* length */)
        {
            if (!ec)
            {
            }
            else
            {
                logger_.error() << "async_write " << ec.message() << std::endl;
            }
        });
}

}  // namespace httpserver