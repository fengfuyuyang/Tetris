#include <SFML/Graphics.hpp>

#include "board.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(320, 480), "SFML Tetris");

    Board board;

    sf::Clock clock;
    float fallTime = 0.5f;
    float deltaTime = 0.0f;

    while (window.isOpen()) {
        deltaTime = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left)
                    board.moveCurrentTetromino(-1, 0);
                else if (event.key.code == sf::Keyboard::Right)
                    board.moveCurrentTetromino(1, 0);
                else if (event.key.code == sf::Keyboard::Down)
                    board.moveCurrentTetromino(0, 1);
                else if (event.key.code == sf::Keyboard::Up)
                    board.rotateCurrentTetromino();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            fallTime = 0.05f;
        else
            fallTime = 0.5f;

        if (board.update(deltaTime, fallTime)) {
            window.close();
        }

        window.clear(sf::Color::White);
        board.draw(window, "images/background.png");
        window.display();
    }

    return 0;
}
