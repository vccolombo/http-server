#include "response.hpp"

#include <fstream>
#include <sstream>

namespace httpserver
{

Response::Response() : status_code_(200) {}

void Response::send(const std::string& data)
{
    data_ = data;
}

// TODO: this might work better as a free function. I'll keep it here for now because I might have
// plans to make this code more complex and require inner access
void Response::send_file(const std::string& path)
{
    std::ifstream file(path);
    if (file.fail())
    {
        // TODO: implement a specialized error
        throw std::runtime_error("Could not open file " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    send(buffer.str());
}

void Response::set_status(Response::status_code_t status_code)
{
    status_code_ = status_code;
}

Response::status_code_t Response::status() const
{
    return status_code_;
}

std::string Response::data() const
{
    return data_;
}

}  // namespace httpserver