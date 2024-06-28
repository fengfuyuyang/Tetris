#pragma once

#include <SFML/Graphics.hpp>

class Square {
public:
    Square(float size, sf::Color color, sf::FloatRect bounds)
            : m_size(size), m_color(color), m_bounds(bounds) {
        m_shape.setSize(sf::Vector2f(m_size, m_size));
        m_shape.setFillColor(color);
        // 不需要边框，因此不设置 outline
        // m_shape.setOutlineThickness(0.0F); // 这一行其实可以省略
    }

    void setPosition(float x, float y) { m_shape.setPosition(x, y); }

    bool canRotate(sf::Vector2f newPos) const {
        return m_bounds.contains(newPos.x, newPos.y) &&
               m_bounds.contains(newPos.x + m_size, newPos.y + m_size);
    }

    bool canMove(float dx, float dy) const {
        sf::Vector2f newPosition = m_shape.getPosition() + sf::Vector2f(dx, dy);
        return m_bounds.contains(newPosition.x, newPosition.y) &&
               m_bounds.contains(newPosition.x + m_size, newPosition.y + m_size);
    }

    void move(float dx, float dy) { m_shape.move(dx, dy); }

    void draw(sf::RenderWindow& window) { window.draw(m_shape); }

    sf::Vector2f getPosition() const { return m_shape.getPosition(); }

private:
    float m_size;               // 方块的边长
    sf::Color m_color;          // 方块的颜色
    sf::RectangleShape m_shape; // 方块的形状
    sf::FloatRect m_bounds;     // 方块的移动范围
};