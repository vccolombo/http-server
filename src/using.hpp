#pragma once

#include <boost/asio.hpp>
#include <memory>

namespace httpserver
{

using boost::asio::io_context;
using boost::asio::ip::tcp;

class Application;
using Application_ptr = std::unique_ptr<Application>;

class Writer;
using Writer_ptr = Writer*;

}  // namespace httpserver