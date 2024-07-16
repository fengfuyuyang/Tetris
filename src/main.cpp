#include "background.h"
#include "block.h"

void Start() {
    Background background(L"images\\background.png", 18);
    int width = background.GetSize().x;
    int height = background.GetSize().y;
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML Tetris");
    Block block(L"images\\tetromino.png");
    block.Init();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    block.Rotate();
                } else if (event.key.code == sf::Keyboard::Down) {
                    block.Move(0, 1);
                } else if (event.key.code == sf::Keyboard::Left) {
                    block.Move(-1, 0);
                } else if (event.key.code == sf::Keyboard::Right) {
                    block.Move(1, 0);
                }
            }
        }

        window.clear(sf::Color::White);
        background.Draw(window);
        block.Draw(window);

        window.display();
    }
}

int main() {
    Start();
    return 0;
}