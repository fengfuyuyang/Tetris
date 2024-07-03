#include "background.h"

void Start() {
    Background background(L"images\\background.png", 18);
    int width = background.GetSize().x;
    int height = background.GetSize().y;
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML Tetris");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // if (!game_over && event.type == sf::Event::KeyPressed) {
            //     if (event.key.code == sf::Keyboard::Up) {
            //         tetromino->Rotate();
            //     } else if (event.key.code == sf::Keyboard::Down) {
            //         tetromino->Move(0, 1);
            //     } else if (event.key.code == sf::Keyboard::Left) {
            //         tetromino->Move(-1, 0);
            //     } else if (event.key.code == sf::Keyboard::Right) {
            //         tetromino->Move(1, 0);
            //     }
            // }
        }

        window.clear(sf::Color::White);
        background.Draw(window);

        window.display();
    }
}

int main() {
    Start();
    return 0;
}