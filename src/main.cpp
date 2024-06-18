#include <SFML/Graphics.hpp>
#include "Board.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(360, 640), "SFML Tetris");

    Board board;

    sf::Clock clock;
    float fallTime = 0.5f; // 控制方块下落时间间隔
    float deltaTime = 0.0f;

    while (window.isOpen()) {
        deltaTime = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // if (event.type == sf::Event::KeyPressed) {
            //     if (event.key.code == sf::Keyboard::Left) {
            //         board.moveCurrentBlock(-1, 0);
            //     } else if (event.key.code == sf::Keyboard::Right) {
            //         board.moveCurrentBlock(1, 0);
            //     } else if (event.key.code == sf::Keyboard::Down) {
            //         board.moveCurrentBlock(0, 1);
            //     } else if (event.key.code == sf::Keyboard::Up) {
            //         board.rotateCurrentBlock();
            //     }
            // }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            fallTime = 0.05f; // 按住向下箭头键可以加速方块下落
        } else {
            fallTime = 0.5f;
        }
        // if (board.update(deltaTime, fallTime)) {
        //     // 游戏结束逻辑
        //     window.close();
        // }

        window.clear(sf::Color::White);
        // board.draw(window);
        window.display();
    }

    return 0;
}
