#include "square.h"

void start() {
    Square square("images/tetromino.png");
    int side_length = square.GetSideLength();

    sf::Texture texture;
    texture.loadFromFile("images/background.png");

    sf::RenderWindow window(sf::VideoMode(texture.getSize().x, texture.getSize().y), "SFML Tetris");


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // if (event.type == sf::Event::KeyPressed) {
            //     if (event.key.code == sf::Keyboard::Up) {
            //     } else if (event.key.code == sf::Keyboard::Down) {

            //     } else if (event.key.code == sf::Keyboard::Left) {

            //     } else if (event.key.code == sf::Keyboard::Right) {

            //     }
            // }
        }

        window.clear(sf::Color::White);
        window.draw(sf::Sprite(texture));

        for (int i = 0; i < square.GetSquareCount(); i++) {
            sf::Sprite sprite = square.GetSprite(i);
            sprite.setPosition(28 + i * side_length, 31 + i * side_length);
            window.draw(sprite);
        }

        window.display();
    }
}

int main() {
    start();
    return 0;
}