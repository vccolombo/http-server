#include "router.hpp"

#include <filesystem>

#include "request.hpp"
#include "response.hpp"

namespace httpserver
{

void Router::register_get(
    const std::string& path, const std::function<void(const Request&, Response&)>& callback)
{
    getters_[path] = callback;
}

void Router::register_static(const std::string& folder_path)
{
    statics_.insert(folder_path);
}

std::function<void(const Request&, Response&)> Router::get(const std::string& path)
{
    // look first at the registered paths, which is a fast operation
    if (getters_.contains(path))
    {
        return getters_.at(path);
    }

    // look second at the filesystem, as it is slower
    for (const auto& static_folder_path : statics_)
    {
        // TODO: refactor
        logger_.debug() << "looking at statics folder: " << static_folder_path << std::endl;
        for (const auto& file_path : std::filesystem::directory_iterator(static_folder_path))
        {
            logger_.debug() << "static file lookup: " << file_path << std::endl;
            logger_.debug() << "comparing paths: " << file_path.path().filename().string() << " "
                            << path << std::endl;
            if ("/" + file_path.path().filename().string() == path)
            {
                return [this, file_path](const Request&, Response& res)
                {
                    auto absolute_file_path = absolute(file_path.path());
                    logger_.info() << "serving static file " << absolute_file_path << std::endl;
                    res.set_status(200);
                    res.send_file(absolute_file_path);
                };
            }
        }
    }

    return {};
}

}  // namespace httpserver