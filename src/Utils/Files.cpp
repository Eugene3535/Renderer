#include "Utils/Files.hpp"

#include <fstream>
#include <filesystem>
#include <algorithm>

std::string FileUtils::getPathToFile(const std::string& filename, const std::string& folder)
{   
    const std::string texture_folder = "res/" + folder;

    for (auto& file : std::filesystem::recursive_directory_iterator(texture_folder))
        if (file.path().stem().string() == filename)
        {
            std::string filepath = file.path().string();
            std::replace(filepath.begin(), filepath.end(), '\\', '/');
            return filepath;
        }           

    return {};
}