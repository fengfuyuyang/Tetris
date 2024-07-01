#pragma once

#include <SFML/Graphics.hpp>

#include "file_handler.h"

class Background {
public:
    explicit Background(const std::wstring& filename, float cell_size) : cell_size(cell_size) {
        std::wstring path = FileHandler::GetFullPath(filename);
        std::string texture_path = FileHandler::ConvertToString(path);
        if (!FileHandler::FileExists(path)) {
            // 处理文件不存在的情况
            throw std::runtime_error("File not found: " + texture_path);
        }

        if (!texture.loadFromFile(texture_path)) {
            throw std::runtime_error("Failed to load texture from file: " + texture_path);
        }
    }

    void Draw(sf::RenderWindow& window) { window.draw(sf::Sprite(texture)); }

    // 获取背景图尺寸
    sf::Vector2u GetSize() const { return texture.getSize(); }

    // 获取网格的边界
    sf::FloatRect GetBounds() const {
        return {static_cast<float>(X), static_cast<float>(Y), (M + 1) * cell_size,
                (N + 1) * cell_size};
    }

private:
    sf::Texture texture;
    // 网格大小
    static const int M = 10;
    static const int N = 20;
    // 网格左上角坐标
    static const int X = 28;
    static const int Y = 31;
    // 网格单元大小
    float cell_size;
};
