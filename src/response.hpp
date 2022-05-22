#pragma once

#include <cstdint>
#include <string>

namespace httpserver
{

class Response
{
   public:
    using status_code_t = uint16_t;

    Response();

    void set_status(status_code_t status_code);

    void send(const std::string& data);
    void send_file(const std::string& path);

    // I should probably make these getters inline for performance,
    // but I'm not that concerned with performance in this pet project,
    // so I'll just choose to implement on the cpp for better physical encapsulation
    [[nodiscard]] status_code_t status() const;
    [[nodiscard]] std::string data() const;

   private:
    status_code_t status_code_;
    std::string data_;
};

}  // namespace httpserver
