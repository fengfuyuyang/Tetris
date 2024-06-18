#pragma once
#include "Block.h"

class Board
{
public:
    Board();
    bool update(float deltaTime, float fallTime);
    void draw(sf::RenderWindow &window);
    void moveCurrentBlock(int x, int y);
    void rotateCurrentBlock();

private:
    bool checkCollision();
    void clearLines();

    sf::Vector2i board[10][20];
    Block currentBlock;
};
