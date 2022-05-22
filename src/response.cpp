#include "response.hpp"

namespace httpserver
{

Response::Response() : status_code_(200) {}

void Response::send(const std::string& data)
{
    data_ = data;
}

void Response::send_file(const std::string& path){
    // TODO
};

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