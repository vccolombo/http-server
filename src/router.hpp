#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "request.hpp"
#include "response.hpp"

namespace httpserver
{

class Router
{
   public:
    void register_get(
        const std::string& path, const std::function<void(Request&, Response&)>& callback);

    std::function<void(Request&, Response&)> get(const std::string& path);

   private:
    // make this a (ordered)map if I ever implement an hierarchical iteration over it
    std::unordered_map<std::string, std::function<void(Request&, Response&)>> getters_;
};

}  // namespace httpserver
