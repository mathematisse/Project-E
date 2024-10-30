#pragma once

#include "lib_log/log.hpp"
#include "AssetsLoader.hpp"
#include <map>
#include <string>
#include <vector>

AssetsLoader::~AssetsLoader()
{
    for (auto &asset : assets) {
        UnloadTexture(asset.second);
    }
    for (auto &image : images) {
        UnloadImage(image.second);
    }
};

std::string AssetsLoader::get_real_path(std::vector<std::string> path_array)
{
    std::string real_path = base_path;
    for (auto &p : path_array) {
        real_path += p + separator;
    }
    real_path.pop_back();
    return real_path;
}

int AssetsLoader::load_asset(std::string path)
{
    Image image = LoadImage(path.c_str());
    if (image.data == nullptr) {
        LOG_ERROR("Error loading image: " + path);
        return 1;
    }
    images[path] = image;
    assets[path] = LoadTextureFromImage(image);
    if (assets[path].id == 0) {
        LOG_ERROR("Error loading texture: " + path);
        return 1;
    }
    assets_id[assets[path].id] = path;
    return 0;
}

int AssetsLoader::load_assets(std::vector<std::vector<std::string>> paths)
{
    for (auto &path_array : paths) {
        std::string path = get_real_path(path_array);
        if (load_asset(path) != 0)
            return 1;
    }
    return 0;
}

Texture2D &AssetsLoader::get_asset(std::vector<std::string> path_array)
{
    std::string path = get_real_path(path_array);
    return assets[path];
}

Texture2D &AssetsLoader::get_asset_from_id(size_t id) { return assets[assets_id[id]]; }

Texture2D &AssetsLoader::get_asset_with_string(std::string path) { return assets[path]; }

Image &AssetsLoader::get_image(std::vector<std::string> path_array)
{
    std::string path = get_real_path(path_array);
    return images[path];
}

Image &AssetsLoader::get_image(std::string path) { return images[path]; }
