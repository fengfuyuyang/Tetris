#pragma once
#include <SFML/Graphics.hpp>

class Block {
public:
    Block();
    bool canMove(int x, int y);
    void move(int x, int y);
    void rotate();
    void draw(sf::RenderWindow &window);
    void setColor(sf::Color color);

private:
    sf::RectangleShape shape;
    int position[4][2];
    int rotation;
};
