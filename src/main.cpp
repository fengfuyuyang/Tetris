/**
 * @Author: fengfuyuyang@outlook.com
 * @File Name: main.cpp
 * @File Version: 0.1
 * @Create Time: 2024-07-17 11:14:29
 * 
 * Copyright (c) 2024 fengfuyuyang@outlook.com. All rights reserved.
 */

#include <vector>

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
    // 经典的任天堂俄罗斯方块版本中，不同等级的下落速度
    const std::vector<float> fall_speeds = {1.0F,  0.93F, 0.85F, 0.78F, 0.70F,
                                            0.63F, 0.55F, 0.48F, 0.40F, 0.30F};
    // 初始等级
    int level = 4;
    float delay = fall_speeds[level];

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
                switch (event.key.code) {
                case sf::Keyboard::Up:
                    block.Rotate();
                    break;
                case sf::Keyboard::Down:
                    block.Move(0, 1);
                    break;
                case sf::Keyboard::Left:
                    block.Move(-1, 0);
                    break;
                case sf::Keyboard::Right:
                    block.Move(1, 0);
                    break;
                case sf::Keyboard::Space:
                    block.DropToBottom();
                    break;
                case sf::Keyboard::Add:
                case sf::Keyboard::Equal:
                    level = level + 1 >= fall_speeds.size() ? fall_speeds.size() - 1 : level + 1;
                    delay = fall_speeds[level];
                    break;
                case sf::Keyboard::Subtract:
                case sf::Keyboard::Hyphen:
                    level = level - 1 < 0 ? 0 : level - 1;
                    delay = fall_speeds[level];
                    break;
                case sf::Keyboard::P: // 按 P 键切换暂停状态
                    block.ChangePauseStatus();
                    break;
                case sf::Keyboard::R: // 按 R 键重新开始游戏
                    block.Reset();
                    timer = 0; // 重置计时器
                    break;
                case sf::Keyboard::Escape: // 按 Esc 键退出游戏
                    window.close();
                    break;
                default:
                    break;
                }
            }
        }

        // 更新方块自动下落的计时器
        if (timer > delay) {
            block.Drop();
            timer = 0;
        }

        if (block.IsGameOver()) {
            // window.close();
        }

        window.clear(sf::Color::White);
        background.Draw(window);
        block.Draw(window);

        window.display();
    }
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    StartGame();
    return 0;
}
