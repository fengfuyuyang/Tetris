#pragma once
#include <SFML/Graphics.hpp>

class Tetromino {
public:
    Tetromino();
    bool canMove(int x, int y, const sf::Vector2i board[10][20]);
    void move(int x, int y);
    void rotate(const sf::Vector2i board[10][20]);
    void draw(sf::RenderWindow& window);
    void reset();

    const int (*getPosition() const)[2];
    sf::Color getColor() const;

private:
    int position[4][2];
    sf::Color color;
    sf::RectangleShape shape[4];
    int rotationIndex;

    void updateShape();
};
