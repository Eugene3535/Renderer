#ifndef FILE_UTILS_HPP
#define FILE_UTILS_HPP

#include <string>

struct FileUtils
{
    static std::string getPathToFile(const std::string& filename);
};

#endif