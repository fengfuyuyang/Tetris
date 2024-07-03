#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <random>

#include "square.h"

class Tetromino {
public:
    explicit Tetromino(Square square, sf::FloatRect area) : _cell_size(square.GetSideLength()) {
        // 检查入参
        if (square.GetSquareCount() <= 0) {
            throw std::invalid_argument("square count must be greater than 0");
        }
        if (area.width <= 0 || area.height <= 0) {
            throw std::invalid_argument("draw area width and height must be greater than 0");
        }

        // 初始化随机数生成器
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, square.GetSquareCount() - 1);
        // 随机选择一个方块
        int n = dis(gen);
        _sprites = {square.GetSprite(n), square.GetSprite(n), square.GetSprite(n),
                    square.GetSprite(n)};

        // 初始化方块的边长和绘制区域
        _side_length = square.GetSideLength();
        _draw_area = sf::FloatRect(area.left, area.top, area.width - _side_length,
                                   area.height - _side_length);
        _move_area = sf::FloatRect(_draw_area.left, _draw_area.top - 2 * _side_length,
                                   _draw_area.width, _draw_area.height + 2 * _side_length);
        _positions[0] = area.left;
        _positions[1] = area.left + _side_length * 9;
        _positions[2] = area.top + _side_length * 20;
    }

    virtual ~Tetromino() = default;

    void Move(int dx, int dy) {
        if (CanMove(dx, dy)) {
            float x = _side_length * dx;
            float y = _side_length * dy;
            for (auto& sprite : _sprites) {
                sprite.move(x, y);
            }
        }
    }

    void Draw(sf::RenderWindow& window) {
        for (auto& sprite : _sprites) {
            sf::Vector2f pos = sprite.getPosition();
            if (_draw_area.contains(pos)) {
                window.draw(sprite);
            }
        }
    }

    virtual void Rotate() = 0;

    bool CanMove(float dx, float dy) {
        float x = _side_length * dx;
        float y = _side_length * dy;
        return std::all_of(_sprites.begin(), _sprites.end(), [x, y, this](const auto& sprite) {
            sf::Vector2f pos = sprite.getPosition() + sf::Vector2f(x, y);
            return _move_area.contains(pos);
            // return pos.x >= _positions[0] && pos.x <= _positions[1] &&  pos.y <= _positions[2];
        });
    }

protected:
    virtual void InitPositions() = 0;
    virtual bool CanRotate() = 0;

    sf::Vector2f RotatePoint90(const sf::Vector2f& center, const sf::Vector2f& pos) {
        sf::Vector2f offset = pos - center;
        float rotated_x = -offset.y;
        float rotated_y = offset.x;
        return center + sf::Vector2f(rotated_x, rotated_y);
    }

    sf::Vector2f RotatePointNeg90(const sf::Vector2f& center, const sf::Vector2f& pos) {
        sf::Vector2f offset = pos - center;
        float rotated_x = offset.y;
        float rotated_y = -offset.x;
        return center + sf::Vector2f(rotated_x, rotated_y);
    }

    std::vector<sf::Sprite> _sprites;
    float _side_length;
    sf::FloatRect _draw_area;
    sf::FloatRect _move_area;
    float _cell_size;
    float _positions[3];
};

class I_Tetromino : public Tetromino {
public:
    explicit I_Tetromino(Square square, sf::FloatRect area) : Tetromino(square, area) {
        InitPositions();
    }

    void InitPositions() override {
        float x = _side_length * 4 + 28;
        float y = 32;
        _sprites[0].setPosition(x, y - 3 * _side_length);
        _sprites[1].setPosition(x, y - 2 * _side_length);
        _sprites[2].setPosition(x, y - _side_length);
        _sprites[3].setPosition(x, y);
    }

    void Rotate() override {
        if (CanRotate()) {
            sf::Vector2f center = _sprites[2].getPosition();
            bool is_neg_90 = (center - _sprites[0].getPosition()).x == 0;
            for (auto& sprite : _sprites) {
                sf::Vector2f pos = sprite.getPosition();
                pos = is_neg_90 ? RotatePointNeg90(center, pos) : RotatePoint90(center, pos);
                sprite.setPosition(pos);
            }
        }
    }

protected:
    bool CanRotate() override {
        sf::Vector2f center = _sprites[2].getPosition();
        bool is_neg_90 = (center - _sprites[0].getPosition()).x == 0;
        return std::all_of(
                _sprites.begin(), _sprites.end(), [is_neg_90, center, this](const auto& sprite) {
                    sf::Vector2f pos = sprite.getPosition();
                    pos = is_neg_90 ? RotatePointNeg90(center, pos) : RotatePoint90(center, pos);
                    return _move_area.contains(pos);
                });
    }
};

class J_Tetromino : public Tetromino {
public:
    explicit J_Tetromino(Square square, sf::FloatRect area) : Tetromino(square, area) {
        InitPositions();
    }

    void InitPositions() override {
        float x = _side_length * 4 + 28;
        float y = 32;
        _sprites[0].setPosition(x + _side_length, y - 2 * _side_length);
        _sprites[1].setPosition(x + _side_length, y - _side_length);
        _sprites[2].setPosition(x + _side_length, y);
        _sprites[3].setPosition(x, y);
    }

    void Rotate() override {
        if (CanRotate()) {
            if (_sprites[3].getPosition().x > _sprites[2].getPosition().x) {
                Move(1, 0);
            }

            sf::Vector2f center = _sprites[1].getPosition();
            for (auto& sprite : _sprites) {
                sf::Vector2f pos = sprite.getPosition();
                pos = RotatePoint90(center, pos);
                sprite.setPosition(pos);
            }

            if (_sprites[3].getPosition().x > _sprites[2].getPosition().x) {
                Move(-1, 0);
            }
        }
    }

protected:
    bool CanRotate() override {
        sf::Vector2f center = _sprites[1].getPosition();
        return std::all_of(_sprites.begin(), _sprites.end(), [center, this](const auto& sprite) {
            sf::Vector2f pos = sprite.getPosition();
            pos = RotatePoint90(center, pos);
            return _move_area.contains(pos);
        });
    }
};

class L_Tetromino : public Tetromino {
public:
    explicit L_Tetromino(Square square, sf::FloatRect area) : Tetromino(square, area) {
        InitPositions();
    }

    void InitPositions() override {
        float x = _side_length * 4 + 28;
        float y = 32;
        _sprites[0].setPosition(x, y - 2 * _side_length);
        _sprites[1].setPosition(x, y - _side_length);
        _sprites[2].setPosition(x, y);
        _sprites[3].setPosition(x + _side_length, y);
    }

    void Rotate() override {
        if (CanRotate()) {
            if (_sprites[3].getPosition().x < _sprites[2].getPosition().x) {
                Move(-1, 0);
            }

            sf::Vector2f center = _sprites[1].getPosition();
            for (auto& sprite : _sprites) {
                sf::Vector2f pos = sprite.getPosition();
                pos = RotatePoint90(center, pos);
                sprite.setPosition(pos);
            }

            if (_sprites[3].getPosition().x < _sprites[2].getPosition().x) {
                Move(1, 0);
            }
        }
    }

protected:
    bool CanRotate() override {
        sf::Vector2f center = _sprites[1].getPosition();
        return std::all_of(_sprites.begin(), _sprites.end(), [center, this](const auto& sprite) {
            sf::Vector2f pos = sprite.getPosition();
            pos = RotatePoint90(center, pos);
            return _move_area.contains(pos);
        });
    }
};

class O_Tetromino : public Tetromino {
public:
    explicit O_Tetromino(Square square, sf::FloatRect area) : Tetromino(square, area) {
        InitPositions();
    }

    void InitPositions() override {
        float x = _side_length * 4 + 28;
        float y = 32;
        _sprites[0].setPosition(x + _side_length, y - _side_length);
        _sprites[1].setPosition(x, y - _side_length);
        _sprites[2].setPosition(x + _side_length, y);
        _sprites[3].setPosition(x, y);
    }

    void Rotate() override {
        // O型方块不需要旋转，因此这里留空
    }

    bool CanRotate() override {
        // O型方块不需要旋转，因此始终返回true
        return true;
    }
};

class S_Tetromino : public Tetromino {
public:
    explicit S_Tetromino(Square square, sf::FloatRect area) : Tetromino(square, area) {
        InitPositions();
    }

    void InitPositions() override {
        float x = _side_length * 4 + 28;
        float y = 32;
        _sprites[0].setPosition(x, y - 2 * _side_length);
        _sprites[1].setPosition(x, y - _side_length);
        _sprites[2].setPosition(x + _side_length, y - _side_length);
        _sprites[3].setPosition(x + _side_length, y);
    }

    void Rotate() override {
        if (CanRotate()) {
            bool is_neg_90 = _sprites[1].getPosition().x == _sprites[2].getPosition().x;

            sf::Vector2f center = _sprites[1].getPosition();
            for (auto& sprite : _sprites) {
                sf::Vector2f pos = sprite.getPosition();
                pos = is_neg_90 ? RotatePointNeg90(center, pos) : RotatePoint90(center, pos);
                sprite.setPosition(pos);
            }
        }
    }

protected:
    bool CanRotate() override {
        sf::Vector2f center = _sprites[1].getPosition();
        bool is_neg_90 = _sprites[1].getPosition().x == _sprites[2].getPosition().x;
        return std::all_of(
                _sprites.begin(), _sprites.end(), [is_neg_90, center, this](const auto& sprite) {
                    sf::Vector2f pos = sprite.getPosition();
                    pos = is_neg_90 ? RotatePointNeg90(center, pos) : RotatePoint90(center, pos);
                    return _move_area.contains(pos);
                });
    }
};

class T_Tetromino : public Tetromino {
public:
    explicit T_Tetromino(Square square, sf::FloatRect area) : Tetromino(square, area) {
        InitPositions();
    }

    void InitPositions() override {
        float x = _side_length * 4 + 28;
        float y = 32;
        _sprites[0].setPosition(x, y - 2 * _side_length);
        _sprites[1].setPosition(x, y - _side_length);
        _sprites[2].setPosition(x, y);
        _sprites[3].setPosition(x + _side_length, y - _side_length);
    }

    void Rotate() override {
        if (CanRotate()) {
            if (_sprites[3].getPosition().x < _sprites[1].getPosition().x) {
                Move(-1, 0);
            }

            sf::Vector2f center = _sprites[1].getPosition();
            for (auto& sprite : _sprites) {
                sf::Vector2f pos = sprite.getPosition();
                pos = RotatePoint90(center, pos);
                sprite.setPosition(pos);
            }

            if (_sprites[3].getPosition().x < _sprites[1].getPosition().x) {
                Move(1, 0);
            }
        }
    }

protected:
    bool CanRotate() override {
        sf::Vector2f center = _sprites[1].getPosition();
        return std::all_of(_sprites.begin(), _sprites.end(), [center, this](const auto& sprite) {
            sf::Vector2f pos = sprite.getPosition();
            pos = RotatePoint90(center, pos);
            return _move_area.contains(pos);
        });
    }
};

class Z_Tetromino : public Tetromino {
public:
    explicit Z_Tetromino(Square square, sf::FloatRect area) : Tetromino(square, area) {
        InitPositions();
    }

    void InitPositions() override {
        float x = _side_length * 4 + 28;
        float y = 32;
        _sprites[0].setPosition(x + _side_length, y - 2 * _side_length);
        _sprites[1].setPosition(x, y - _side_length);
        _sprites[2].setPosition(x + _side_length, y - _side_length);
        _sprites[3].setPosition(x, y);
    }

    void Rotate() override {
        if (CanRotate()) {
            bool is_neg_90 = _sprites[1].getPosition().x == _sprites[2].getPosition().x;

            sf::Vector2f center = _sprites[1].getPosition();
            for (auto& sprite : _sprites) {
                sf::Vector2f pos = sprite.getPosition();
                pos = is_neg_90 ? RotatePointNeg90(center, pos) : RotatePoint90(center, pos);
                sprite.setPosition(pos);
            }
        }
    }

protected:
    bool CanRotate() override {
        sf::Vector2f center = _sprites[1].getPosition();
        bool is_neg_90 = _sprites[1].getPosition().x == _sprites[2].getPosition().x;
        return std::all_of(
                _sprites.begin(), _sprites.end(), [is_neg_90, center, this](const auto& sprite) {
                    sf::Vector2f pos = sprite.getPosition();
                    pos = is_neg_90 ? RotatePointNeg90(center, pos) : RotatePoint90(center, pos);
                    return _move_area.contains(pos);
                });
    }
};
