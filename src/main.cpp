#include "background.h"
#include "square.h"
#include "tetromino.h"

void start() {
    Square square(L"images\\tetromino.png");
    int side_length = square.GetSideLength();
    Background background(L"images\\background.png", side_length);
    int width = background.GetSize().x;
    int height = background.GetSize().y;
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML Tetris");
    I_Tetromino tetromino(square, background.GetBounds());

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    tetromino.Rotate();
                } else if (event.key.code == sf::Keyboard::Down) {
                    tetromino.Move(0, 1);
                } else if (event.key.code == sf::Keyboard::Left) {
                    tetromino.Move(-1, 0);
                } else if (event.key.code == sf::Keyboard::Right) {
                    tetromino.Move(1, 0);
                }
            }
        }

        window.clear(sf::Color::White);
        background.Draw(window);
        tetromino.Draw(window);

        window.display();
    }
}

int main() {
    start();
    return 0;
}
