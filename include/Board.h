#pragma once
#include <SFML/Graphics.hpp>

#include "tetromino.h"

class Board {
public:
    Board();
    bool update(float deltaTime, float fallTime);
    void draw(sf::RenderWindow& window, const std::string& background_image_path);
    void moveCurrentTetromino(int x, int y);
    void rotateCurrentTetromino();

private:
    bool checkCollision();
    void clearLines();

    sf::Vector2i board[10][20];
    Tetromino currentTetromino;
    float currentTime;
};
