#pragma once

#include <boost/asio.hpp>
#include <cstdint>
#include <memory>

#include "application.hpp"
#include "logger.hpp"
#include "writer.hpp"

namespace httpserver
{

using boost::asio::ip::tcp;

class Connection final : public Writer, public std::enable_shared_from_this<Connection>
{
   public:
    Connection(tcp::socket socket, Application_ptr app);

    void accept();

   private:
    void read();
    void write(uint8_t* data, std::size_t length) override;

    uint8_t* get_data_from_buffer(std::size_t n_bytes);

    tcp::socket socket_;
    Application_ptr app_;
    boost::asio::streambuf buffer_;
    logging::Logger logger_{logging::create_logger("Connection")};
};

}  // namespace httpserver
