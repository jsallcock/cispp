#pragma once

#include <filesystem>
#include <stdexcept>

namespace cispp {

std::filesystem::path getRootPath()
{
    const char* rootPath = std::getenv("CISPP_ROOT");
    if (rootPath)
    {
        std::string rootPathOut(rootPath);
        return std::filesystem::path(rootPathOut);
    }
    throw std::runtime_error("ERROR: environment variable 'CISPP_ROOT' was not found.");
};

} // namespace cispp