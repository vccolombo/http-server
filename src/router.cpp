#include "router.hpp"

namespace httpserver
{

void Router::register_get(
    const std::string& path, const std::function<void(Request&, Response&)>& callback)
{
    getters_[path] = callback;
}

std::function<void(Request&, Response&)> Router::get(const std::string& path)
{
    // I'm presuming this returns an empty function if it was not registered before. Need to
    // test but it's still a bad idea, as it'll increase the size of the map. So:
    // TODO: check if key exists
    return getters_[path];
}

}  // namespace httpserver