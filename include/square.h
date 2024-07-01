#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "file_handler.h"

class Square {
public:
    explicit Square(const std::wstring& filename) {
        std::wstring path = FileHandler::GetFullPath(filename);
        std::string texture_path = FileHandler::ConvertToString(path);
        if (!FileHandler::FileExists(path)) {
            // 处理文件不存在的情况
            throw std::runtime_error("File not found: " + texture_path);
        }

        // 加载纹理
        if (!m_texture.loadFromFile(texture_path)) {
            // 处理加载失败的情况
            throw std::runtime_error("Failed to load texture from " + texture_path);
        }

        // 创建所有正方形的Sprite
        CreateSprites();
    }

    // 获取正方形的边长
    int GetSideLength() const { return m_texture.getSize().y; }

    // 获取正方形的数量
    int GetSquareCount() const { return m_texture.getSize().x / GetSideLength(); }

    // 获取指定编号的正方形的Sprite
    sf::Sprite GetSprite(int n) const {
        if (n < 0 || n >= GetSquareCount()) {
            throw std::out_of_range("Invalid square index: " + std::to_string(n));
        }

        return m_sprites[n];
    }

private:
    // 创建所有正方形的Sprite
    void CreateSprites() {
        // 遍历所有正方形的编号
        for (int i = 0; i < GetSquareCount(); ++i) {
            // 创建Sprite并设置纹理
            sf::Sprite sprite(m_texture);

            // 设置纹理矩形, 左上角坐标为(i * GetSideLength(), 0), 大小为(GetSideLength(), GetSideLength())
            sprite.setTextureRect(
                    sf::IntRect(i * GetSideLength(), 0, GetSideLength(), GetSideLength()));
            m_sprites.push_back(sprite);
        }
    }

    // 纹理
    sf::Texture m_texture;
    // 所有正方形的Sprite
    std::vector<sf::Sprite> m_sprites;
};
