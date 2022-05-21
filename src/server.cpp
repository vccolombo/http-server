#include "server.hpp"

namespace httpserver
{

TCPServer::TCPServer(io_context& io_context, tcp::endpoint& endpoint, Application& app)
    : acceptor_(io_context, endpoint), app_(app) {}


}  // namespace httpserver