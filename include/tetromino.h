#pragma once

#include <algorithm>
#include <random>

#include "square.h"

class Tetromino {
public:
    explicit Tetromino(Square square, sf::FloatRect area) : area(area) {
        // 检查入参
        if (square.GetSquareCount() <= 0) {
            throw std::invalid_argument("square count must be greater than 0");
        }
        if (area.width <= 0 || area.height <= 0) {
            throw std::invalid_argument("area width and height must be greater than 0");
        }

        // 随机数生成器
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, square.GetSquareCount() - 1);
        // 随机选择一个方块
        int n = dis(gen);
        sprites = {square.GetSprite(n), square.GetSprite(n), square.GetSprite(n),
                   square.GetSprite(n)};
        // 方块的边长
        side_length = square.GetSideLength();
    }
    virtual void InitPositions() = 0;
    virtual void Rotate() = 0;

    // 移动一个单位距离
    void Move(int dx, int dy) {
        float x = side_length * dx;
        float y = side_length * dy;

        if (CanMove(x, y)) {
            for (auto& sprite : sprites) {
                sprite.move(x, y);
            }
        }
    }

    void Draw(sf::RenderWindow& window) {
        for (auto& sprite : sprites) {
            // 若超出边界，则不绘制
            sf::Vector2f pos = sprite.getPosition();
            if (area.contains(pos) && area.contains(pos + sf::Vector2f(side_length, side_length))) {
                window.draw(sprite);
            }
        }
    }

protected:
    // 判断是否可以移动
    bool CanMove(float dx, float dy) {
        // 遍历所有方块，判断是否有方块超出边界
        // 若有，则不能移动
        return std::all_of(sprites.begin(), sprites.end(), [dx, dy, this](const auto& sprite) {
            sf::Vector2f pos = sprite.getPosition() + sf::Vector2f(dx, dy);
            return area.contains(pos) &&
                   area.contains(pos + sf::Vector2f(side_length, side_length));
        });
    }

    bool CanRotate() {
        // 遍历所有方块，判断是否有方块超出边界
        // 若有，则不能旋转
        return std::all_of(sprites.begin(), sprites.end(), [this](const auto& sprite) {
            sf::Vector2f center =
                    sprite.getPosition() + sf::Vector2f(sprite.getGlobalBounds().width / 2.0F,
                                                        sprite.getGlobalBounds().height / 2.0F);
            sf::Vector2f pos = center + sf::Vector2f(side_length, 0);
            return area.contains(pos) &&
                   area.contains(pos + sf::Vector2f(side_length, side_length));
        });
    }

    sf::FloatRect area;
    std::vector<sf::Sprite> sprites;

    // 方块的边长
    float side_length;
};

class I_Tetromino : public Tetromino {
public:
    explicit I_Tetromino(Square square, const sf::FloatRect& area) : Tetromino(square, area) {
        InitPositions();
    }

    void InitPositions() override {
        // 初始化T型方块中的四个方块的位置

        float x = side_length * 4 + 28;
        float y = 32;
        sprites[0].setPosition(x - side_length, y);
        sprites[1].setPosition(x, y);
        sprites[2].setPosition(x + side_length, y);
        sprites[3].setPosition(x + side_length * 2, y);
    }

    void Rotate() override {
        // 旋转I型方块

        if (!CanRotate()) {
            return;
        }

        // 旋转四个方块
        for (auto& sprite : sprites) {
            sf::Vector2f center =
                    sprite.getPosition() + sf::Vector2f(sprite.getGlobalBounds().width / 2.0F,
                                                        sprite.getGlobalBounds().height / 2.0F);
            sf::Vector2f pos = center + sf::Vector2f(side_length, 0);
            sprite.setPosition(pos);
        }
    }
};

// class J_Tetromino : public Tetromino {
// public:
//     explicit J_Tetromino(Square square) : Tetromino(square) {}
// };

// class L_Tetromino : public Tetromino {
// public:
//     explicit L_Tetromino(Square square) : Tetromino(square) {}
// };

// class O_Tetromino : public Tetromino {
// public:
//     explicit O_Tetromino(Square square) : Tetromino(square) {}
// };

// class S_Tetromino : public Tetromino {
// public:
//     explicit S_Tetromino(Square square) : Tetromino(square) {}
// };

// class T_Tetromino : public Tetromino {
// public:
//     explicit T_Tetromino(Square square, const sf::FloatRect& area) : Tetromino(square, area) {
//         InitPositions();
//     }

//     void InitPositions() override {
//     }

//     void Rotate() override {
//     }
// };

// class Z_Tetromino : public Tetromino {
// public:
//     explicit Z_Tetromino(Square square) : Tetromino(square) {}
// };
