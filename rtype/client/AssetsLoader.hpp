#include "raylib.h"
#include <map>
#include <string>
#include <vector>
#include <iostream>

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
    ~AssetsLoader()
    {
        for (auto &asset : assets) {
            UnloadTexture(asset.second);
        }
        for (auto &image : images) {
            UnloadImage(image.second);
        }
    };

private:
    std::map<std::string, Image> images;
    std::map<std::string, Texture2D> assets;
    std::map<size_t, std::string> assets_id;
    std::vector<Texture2D> tiles;

public:
    std::string get_real_path(std::vector<std::string> path_array)
    {
        std::string real_path = base_path;
        for (auto &p : path_array) {
            real_path += p + separator;
        }
        real_path.pop_back();
        return real_path;
    }

    int load_asset(std::string path)
    {
        Image image = LoadImage(path.c_str());
        if (image.data == nullptr) {
            std::cerr << "Error loading image: " << path << std::endl;
            return 1;
        }
        images[path] = image;
        assets[path] = LoadTextureFromImage(image);
        if (assets[path].id == 0) {
            std::cerr << "Error loading texture: " << path << std::endl;
            return 1;
        }
        assets_id[assets[path].id] = path;
        return 0;
    }

    int load_assets(std::vector<std::vector<std::string>> paths)
    {
        for (auto &path_array : paths) {
            std::string path = get_real_path(path_array);
            if (load_asset(path) != 0)
                return 1;
        }
        return 0;
    }

    Texture2D &get_asset(std::vector<std::string> path_array)
    {
        std::string path = get_real_path(path_array);
        return assets[path];
    }

    Texture2D &get_asset_from_id(size_t id) { return assets[assets_id[id]]; }

    Texture2D &get_asset_with_string(std::string path) { return assets[path]; }

    Image &get_image(std::vector<std::string> path_array)
    {
        std::string path = get_real_path(path_array);
        return images[path];
    }

    Image &get_image(std::string path) { return images[path]; }
};
