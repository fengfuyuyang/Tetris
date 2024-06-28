#include "Tetromino.h"

#include <ctime>

Tetromino::Tetromino() {
    srand(time(0));
    reset();
}

void Tetromino::reset() {
    // 定义所有Tetromino的形状和颜色
    int shapes[7][4][2] = {
            {{0, 0}, {1, 0}, {0, 1}, {1, 1}}, // O
            {{0, 0}, {1, 0}, {2, 0}, {3, 0}}, // I
            {{0, 0}, {0, 1}, {1, 1}, {2, 1}}, // L
            {{2, 0}, {0, 1}, {1, 1}, {2, 1}}, // J
            {{1, 0}, {2, 0}, {0, 1}, {1, 1}}, // S
            {{0, 0}, {1, 0}, {1, 1}, {2, 1}}, // Z
            {{1, 0}, {0, 1}, {1, 1}, {2, 1}}  // T
    };

    sf::Color colors[7] = {sf::Color::Yellow, sf::Color::Cyan,    sf::Color::Blue, sf::Color::Red,
                           sf::Color::Green,  sf::Color::Magenta, sf::Color::White};

    int shapeIndex = rand() % 7;
    color = colors[shapeIndex];

    for (int i = 0; i < 4; ++i) {
        position[i][0] = shapes[shapeIndex][i][0] + 3;
        position[i][1] = shapes[shapeIndex][i][1];
        shape[i].setSize(sf::Vector2f(32, 32));
        shape[i].setFillColor(color);
    }

    rotationIndex = 0;
    updateShape();
}

bool Tetromino::canMove(int x, int y, const sf::Vector2i board[10][20]) {
    for (int i = 0; i < 4; ++i) {
        int newX = position[i][0] + x;
        int newY = position[i][1] + y;
        if (newX < 0 || newX >= 10 || newY >= 20 || newY < 0 ||
            board[newX][newY] != sf::Vector2i(0, 0))
            return false;
    }
    return true;
}

void Tetromino::move(int x, int y) {
    for (int i = 0; i < 4; ++i) {
        position[i][0] += x;
        position[i][1] += y;
    }
    updateShape();
}

void Tetromino::rotate(const sf::Vector2i board[10][20]) {
    // 简单的旋转实现，仅供参考
    int oldPos[4][2];
    memcpy(oldPos, position, sizeof(position));

    int pivotX = position[1][0];
    int pivotY = position[1][1];

    for (int i = 0; i < 4; ++i) {
        int x = position[i][1] - pivotY;
        int y = position[i][0] - pivotX;
        position[i][0] = pivotX - x;
        position[i][1] = pivotY + y;
    }

    if (!canMove(0, 0, board)) {
        memcpy(position, oldPos, sizeof(position));
    }

    updateShape();
}

// void Tetromino::draw(sf::RenderWindow& window) {
//     for (int i = 0; i < 4; ++i) {
//         window.draw(shape[i]);
//     }
// }

void Tetromino::updateShape() {
    for (int i = 0; i < 4; ++i) {
        shape[i].setPosition(position[i][0] * 32, position[i][1] * 32);
    }
}

const int (*Tetromino::getPosition() const)[2] {
    return position;
}

sf::Color Tetromino::getColor() const {
    return color;
}
