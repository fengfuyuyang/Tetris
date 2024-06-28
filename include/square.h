#pragma once

#include <SFML/Graphics.hpp>

class Square {
public:
    explicit Square(const std::string& texture_path) {
        if (!m_texture.loadFromFile(texture_path)) {
            // 处理加载失败的情况
            throw std::runtime_error("Failed to load texture from " + texture_path);
        }
    }

    // 获取正方形的边长
    int GetSideLength() const {
        return m_texture.getSize().y;
    }

    // 获取正方形的数量
    int GetSquareCount() const {
        return m_texture.getSize().x / GetSideLength();
    }

    // 获取指定编号的正方形的Sprite
    sf::Sprite GetSprite(int n) const {
        if (n < 0 || n >= GetSquareCount()) {
            throw std::out_of_range("Invalid square index: " + std::to_string(n));
        }

        // 创建Sprite并设置纹理
        sf::Sprite sprite(m_texture);

        // 设置纹理矩形, 左上角坐标为(n * GetSideLength(), 0), 大小为(GetSideLength(), GetSideLength())
        sprite.setTextureRect(sf::IntRect(n * GetSideLength(), 0, GetSideLength(), GetSideLength()));
        return sprite;
    }

private:
    // 纹理
    sf::Texture m_texture;
};
