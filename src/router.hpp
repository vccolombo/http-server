#pragma once

#include <functional>
#include <set>
#include <string>
#include <unordered_map>

#include "logger.hpp"

namespace httpserver
{

class Request;
class Response;

class Router
{
   public:
    void register_get(
        const std::string& path, const std::function<void(const Request&, Response&)>& callback);
    void register_static(const std::string& folder_path);

    std::function<void(const Request&, Response&)> get(const std::string& path);

   private:
    // make this a (ordered)map if I ever implement a hierarchical iteration over it
    std::unordered_map<std::string, std::function<void(const Request&, Response&)>> getters_;
    // this is ordered because the order of registration is important
    // e.g. register_static("/1"); register_static("/2"); should always look at "/1" first
    std::set<std::string> statics_;
    logging::Logger logger_{logging::create_logger("Router")};
};

}  // namespace httpserver
