/**
 * @Author: fengfuyuyang@outlook.com
 * @File Name: main.cpp
 * @File Version: 0.1
 * @Create Time: 2024-07-17 11:14:29
 * 
 * Copyright (c) 2024 fengfuyuyang@outlook.com. All rights reserved.
 */

#include "background.h"
#include "block.h"

void StartGame() {
    Background background(L"images\\background.png", 18);
    int width = background.GetSize().x;
    int height = background.GetSize().y;
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML Tetris");
    Block block(L"images\\tetromino.png");
    block.GetNewBlock();

    // 创建时钟
    sf::Clock clock;
    float timer = 0;
    float delay = 0.3;

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
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
                } else if (event.key.code == sf::Keyboard::Space) {
                    block.DropToBottom();
                }
            }
        }

        // 更新方块自动下落的计时器
        if (timer > delay) {
            block.Drop();
            timer = 0;
        }

        if (block.IsGameOver()) {
            window.close();
        }

        window.clear(sf::Color::White);
        background.Draw(window);
        block.Draw(window);

        window.display();
    }
}

int main() {
    StartGame();
    return 0;
}