#pragma once

#include "lib_log/log.hpp"
#include "raylib.h"
#include <map>
#include <string>
#include <vector>

#ifdef _WIN32 // Windows
const std::string separator = "\\";
const std::string temp_path = GetWorkingDirectory();
std::vector<std::string> splitString(const std::string &str, const std::string &delimiter)
{
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    result.push_back(str.substr(start));

    return result;
}
const std::string base_path = splitString(temp_path, "out\\")[0];
#else // Linux / Unix
const std::string separator = "/";
const std::string base_path = "";
#endif

class AssetsLoader {
public:
    AssetsLoader() = default;
    ~AssetsLoader();

private:
    std::map<std::string, Image> images;
    std::map<std::string, Texture2D> assets;
    std::map<size_t, std::string> assets_id;
    std::vector<Texture2D> tiles;

public:
    std::string get_real_path(std::vector<std::string> path_array);

    int load_asset(std::string path);

    int load_assets(std::vector<std::vector<std::string>> paths);

    Texture2D &get_asset(std::vector<std::string> path_array);

    Texture2D &get_asset_from_id(size_t id) { return assets[assets_id[id]]; }

    Texture2D &get_asset_with_string(std::string path);

    Image &get_image(std::vector<std::string> path_array);

    Image &get_image(std::string path);
};
