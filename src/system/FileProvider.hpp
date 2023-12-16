#ifndef FILE_PROVIDER_HPP
#define FILE_PROVIDER_HPP

#include <string>

struct FileProvider
{
    std::string getPathToFile(const std::string& filename) noexcept;
};

#endif