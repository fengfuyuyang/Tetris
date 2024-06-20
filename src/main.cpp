#include <SFML/Graphics.hpp>

#include "board.h"

void set_window() {
    // 定义网格的大小和尺寸
    int gridSize = 20; // 每个网格的边长
    int gridWidth = 40; // 网格的列数
    int gridHeight = 30; // 网格的行数

    // 计算窗口的宽度和高度
    int gridToalWidth = gridWidth * gridSize;
    int gridToalHeight = gridHeight * gridSize;

    // 上方额外空间格子数
    int ExtrHeightGridSize = 3;

    // 下方额外空间格子数
    int ExtrBottomGridSize = 2;

    // 左侧额外空间格子数
    int ExtrLeftGridSize = 5;

    // 右侧额外空间格子数
    int ExtrWidthGridSize = 5;

    // 计算窗口的宽度和高度
    int windowWidth = gridToalWidth + (ExtrLeftGridSize + ExtrWidthGridSize) * gridSize;
    int windowHeight = gridToalHeight + (ExtrHeightGridSize + ExtrBottomGridSize) * gridSize;

    // 设置窗口大小
    // 创建窗口
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML Tetris");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // 清空窗口
        window.clear(sf::Color::White);

        // 绘制网格
        sf::VertexArray lines(sf::Lines);

        // 绘制垂直线
        for (int i = ExtrWidthGridSize; i <= gridWidth + ExtrWidthGridSize; ++i) {
            // 垂直线上方顶点
            lines.append(sf::Vertex(sf::Vector2f(i * gridSize, gridSize * ExtrHeightGridSize), sf::Color::Black));
            // 垂直线下方顶点
            lines.append(sf::Vertex(sf::Vector2f(i * gridSize, windowHeight - gridSize * ExtrBottomGridSize), sf::Color::Black));
        }

        // 绘制水平线
        for (int i = ExtrHeightGridSize; i <= gridHeight + ExtrHeightGridSize; ++i) {
            // 水平线左侧顶点
            lines.append(sf::Vertex(sf::Vector2f(gridSize * ExtrLeftGridSize, i * gridSize), sf::Color::Black));
            // 水平线右侧顶点
            lines.append(sf::Vertex(sf::Vector2f(windowWidth - gridSize * ExtrWidthGridSize, i * gridSize), sf::Color::Black));
        }

        // 绘制网格线
        window.draw(lines);

        // 显示绘制的内容
        window.display();
    }
}

// 方块移动
void draw_block() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Square Movement");

    // 设置方块的边长和颜色
    float squareSize = 20.0f;
    sf::Color squareColor(255, 0, 0); // 红色

    // 创建方块
    sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
    square.setFillColor(squareColor);
    square.setOutlineThickness(1.0f);
    square.setOutlineColor(sf::Color::Black);
    square.setPosition(400.0f, 300.0f); // 初始位置

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // 处理键盘输入
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            if (square.getPosition().y - 1.0f >= 0) {
                square.move(0.0f, -1.0f);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            if (square.getPosition().y + squareSize + 1.0f <= window.getSize().y) {
                square.move(0.0f, 1.0f);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            if (square.getPosition().x - 1.0f >= 0) {
                square.move(-1.0f, 0.0f);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            if (square.getPosition().x + squareSize + 1.0f <= window.getSize().x) {
                square.move(1.0f, 0.0f);
            }
        }

        // 清屏
        window.clear(sf::Color::White);
        // 绘制方块
        window.draw(square);
        // 显示绘制内容
        window.display();
    }
}


void tetris() {
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
}

int main() {
    // tetris();
    // set_window();
    draw_block();
    return 0;
}
