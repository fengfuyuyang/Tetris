#include <SFML/Graphics.hpp>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
// #include "board.h"

class Square {
public:
    Square(float size, sf::Color color, sf::FloatRect bounds)
        : m_size(size), m_color(color), m_bounds(bounds) {
        m_shape.setSize(sf::Vector2f(m_size, m_size));
        m_shape.setFillColor(color);
        // 不需要边框，因此不设置 outline
        m_shape.setOutlineThickness(0.0F); // 这一行其实可以省略
    }

    void setPosition(float x, float y) {
        m_shape.setPosition(x, y);
    }

    bool canRotate(sf::Vector2f newPos) const {
        return m_bounds.contains(newPos.x, newPos.y) &&
               m_bounds.contains(newPos.x + m_size, newPos.y + m_size);
    }

    bool canMove(float dx, float dy) const {
        sf::Vector2f newPosition = m_shape.getPosition() + sf::Vector2f(dx, dy);
        return m_bounds.contains(newPosition.x, newPosition.y) &&
               m_bounds.contains(newPosition.x + m_size, newPosition.y + m_size);
    }

    void move(float dx, float dy) {
        m_shape.move(dx, dy);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(m_shape);
    }

    sf::Vector2f getPosition() const {
        return m_shape.getPosition();
    }

private:
    float m_size;               // 方块的边长
    sf::Color m_color;          // 方块的颜色
    sf::RectangleShape m_shape; // 方块的形状
    sf::FloatRect m_bounds;     // 方块的移动范围
};

class Tetromino {
public:
    Tetromino(const std::vector<sf::Vector2f>& positions, float size, sf::Color color, sf::FloatRect bounds)
        : m_size(size), m_color(color), m_bounds(bounds) {
        for (const auto& pos : positions) {
            Square square(size, color, bounds);
            square.setPosition(pos.x, pos.y);
            m_squares.push_back(square);
        }
    }

    void draw(sf::RenderWindow& window) {
        for (auto& square : m_squares) {
            square.draw(window);
        }
    }

    bool canMove(float dx, float dy) const {
        return std::all_of(m_squares.begin(), m_squares.end(),
                           [dx, dy](const auto& square) { return square.canMove(dx, dy); });
    }

    void move(float dx, float dy) {
        if (canMove(dx, dy)) {
            for (auto& square : m_squares) {
                square.move(dx, dy);
            }
        }
    }

    // void rotate() {
    //     // 简单的旋转实现: 以第一个方块为中心旋转其他方块 90 度
    //     if (m_squares.size() < 2) {
    //         return;
    //     }

    //     sf::Vector2f center = m_squares[0].getPosition();
    //     for (size_t i = 1; i < m_squares.size(); ++i) {
    //         float x = m_squares[i].getPosition().x - center.x;
    //         float y = m_squares[i].getPosition().y - center.y;
    //         m_squares[i].setPosition(center.x - y, center.y + x);
    //     }
    // }

    void rotate() {
        if (m_squares.size() < 3) {
            return;
        }

        sf::Vector2f center = m_squares[2].getPosition();
        std::vector<sf::Vector2f> newPositions(m_squares.size());

        for (size_t i = 0; i < m_squares.size(); ++i) {
            if (i == 2) {
                newPositions[i] = center;
                continue;
            }
            float x = m_squares[i].getPosition().x - center.x;
            float y = m_squares[i].getPosition().y - center.y;
            newPositions[i] = sf::Vector2f(center.x - y, center.y + x);

            if (!m_squares[i].canRotate(newPositions[i])) {
                return;
            }
        }

        for (size_t i = 0; i < m_squares.size(); ++i) {
            m_squares[i].setPosition(newPositions[i].x, newPositions[i].y);
        }
    }

private:
    std::vector<Square> m_squares;
    float m_size;
    sf::Color m_color;
    sf::FloatRect m_bounds;
};

class GridDrawer {
public:
    GridDrawer(int rows, int cols, int marginTop, int marginBottom, int marginLeft, int marginRight,
               sf::RenderWindow& window)
            : rows(rows),
              cols(cols),
              marginTop(marginTop),
              marginBottom(marginBottom),
              marginLeft(marginLeft),
              marginRight(marginRight),
              window(window) {
        calculateCellSize();
    }

    void draw() {
        float lineThickness = 1.5F;
        // 绘制网格
        sf::RectangleShape line;

        // 设置垂直线的大小和颜色
        line.setFillColor(sf::Color(128, 128, 128));
        line.setSize(sf::Vector2f(lineThickness, cellSize * rows));

        // 绘制垂直线
        for (int i = 0; i <= cols; ++i) {
            line.setPosition(marginLeft + i * cellSize, marginTop);
            window.draw(line);
        }

        // 设置水平线的大小和颜色
        line.setSize(sf::Vector2f(cellSize * cols, lineThickness));

        // 绘制水平线
        for (int i = 0; i <= rows; ++i) {
            line.setPosition(marginLeft, marginTop + i * cellSize);
            window.draw(line);
        }
    }

    // 获取网格的每个格子的边长
    float getCellSize() const {
        return cellSize;
    }

    // 获取网格的边界
    sf::FloatRect getBounds() const {
        return {static_cast<float>(marginLeft), static_cast<float>(marginTop),
                (cols + 1) * cellSize, (rows + 1) * cellSize};
    }

private:
    int rows, cols;
    int marginTop, marginBottom, marginLeft, marginRight;
    float cellSize;
    sf::RenderWindow& window;

    void calculateCellSize() {
        int availableWidth = window.getSize().x - marginLeft - marginRight;
        int availableHeight = window.getSize().y - marginTop - marginBottom;
        cellSize = std::min(availableWidth / cols, availableHeight / rows);
    }
};

void set_window() {
    // 网格的行数和列数
    int rows = 20;
    int cols = 10;
    // 网格的边距
    int marginTop = 50;    // 上边距
    int marginBottom = 50; // 下边距
    int marginLeft = 50;   // 左边距
    int marginRight = 50;  // 右边距

    sf::RenderWindow window(sf::VideoMode(300, 500), "SFML Tetris");

    GridDrawer gridDrawer(rows, cols, marginTop, marginBottom, marginLeft, marginRight, window);

    float gridSize = gridDrawer.getCellSize();

    float firstLineX = marginLeft + gridSize * (cols / 2 - 1);
    float firstLineY = marginTop + gridSize * 2;

    // T形状的方块
    //  #
    // ##
    //  #
    std::vector<sf::Vector2f> squareShape_T = {{firstLineX, firstLineY},
                                             {firstLineX - gridSize, firstLineY + gridSize},
                                             {firstLineX, firstLineY + gridSize},
                                             {firstLineX, firstLineY + gridSize * 2}};

    // J形状的方块
    //  #
    //  #
    // ##
    std::vector<sf::Vector2f> squareShape_J = {{firstLineX, firstLineY},
                                             {firstLineX - gridSize, firstLineY + gridSize * 2},
                                             {firstLineX, firstLineY + gridSize},
                                             {firstLineX, firstLineY + gridSize * 2}};

    // L形状的方块
    std::vector<sf::Vector2f> squareShape_L = {{firstLineX, firstLineY},
                                             {firstLineX + gridSize, firstLineY - gridSize},
                                             {firstLineX + gridSize, firstLineY},
                                             {firstLineX, firstLineY - gridSize}};

    // O形状的方块
    std::vector<sf::Vector2f> squareShape_O = {{firstLineX, firstLineY},
                                             {firstLineX - gridSize, firstLineY - gridSize},
                                             {firstLineX, firstLineY - gridSize},
                                             {firstLineX - gridSize, firstLineY - gridSize}};

    // S形状的方块
    std::vector<sf::Vector2f> squareShape_S = {{firstLineX, firstLineY},
                                             {firstLineX - gridSize, firstLineY - gridSize},
                                             {firstLineX, firstLineY - gridSize},
                                             {firstLineX + gridSize, firstLineY - gridSize}};

    // Z形状的方块
    std::vector<sf::Vector2f> squareShape_Z = {{firstLineX, firstLineY},
                                             {firstLineX + gridSize, firstLineY - gridSize},
                                             {firstLineX, firstLineY - gridSize},
                                             {firstLineX - gridSize, firstLineY - gridSize}};

    Tetromino tetromino_T(squareShape_T, gridSize, sf::Color::Green, gridDrawer.getBounds());
    Tetromino tetromino_J(squareShape_J, gridSize, sf::Color::Blue, gridDrawer.getBounds());
    Tetromino tetromino_L(squareShape_L, gridSize, sf::Color::Yellow, gridDrawer.getBounds());
    Tetromino tetromino_O(squareShape_O, gridSize, sf::Color::Cyan, gridDrawer.getBounds());
    Tetromino tetromino_S(squareShape_S, gridSize, sf::Color::Magenta, gridDrawer.getBounds());
    Tetromino tetromino_Z(squareShape_Z, gridSize, sf::Color::Red, gridDrawer.getBounds());

    // // 随机选择一个初始形状
    // std::vector<Tetromino*> tetrominoes = {&tetromino_T, &tetromino_J, &tetromino_L, &tetromino_O, &tetromino_S,
    //                                       &tetromino_Z};
    // std::random_shuffle(tetrominoes.begin(), tetrominoes.end());
    // Tetromino* currentTetromino = tetrominoes[0];

    // // 绘制初始形状
    // currentTetromino->draw(window);
    // gridDrawer.draw();
    // window.display();

    // Square square(gridSize, sf::Color::Red, gridDrawer.getBounds());
    // square.setPosition(marginLeft + gridDrawer.getCellSize() * (cols / 2 - 1), marginTop);

    // 创建时钟
    sf::Clock clock;
    sf::Time timeSinceLastMove = sf::Time::Zero;
    sf::Time timePerMove = sf::seconds(0.5F);

    Tetromino tetromino = tetromino_T;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    tetromino.rotate();
                } else if (event.key.code == sf::Keyboard::Down) {
                    if (tetromino.canMove(0.0F, gridSize)) {
                        tetromino.move(0.0F, gridSize);
                    }
                } else if (event.key.code == sf::Keyboard::Left) {
                    if (tetromino.canMove(-gridSize, 0.0F)) {
                        tetromino.move(-gridSize, 0.0F);
                    }
                } else if (event.key.code == sf::Keyboard::Right) {
                    if (tetromino.canMove(gridSize, 0.0F)) {
                        tetromino.move(gridSize, 0.0F);
                    }
                }
            }
        }

        // 更新方块自动下落的计时器
        timeSinceLastMove += clock.restart();
        if (timeSinceLastMove >= timePerMove) {
            if (tetromino.canMove(0.0F, gridSize)) {
                tetromino.move(0.0F, gridSize);
            }
            timeSinceLastMove = sf::Time::Zero;
        }

        // 清空窗口
        // 这里的颜色可以改成其他颜色
        window.clear(sf::Color(30, 30, 30));

        // 绘制方块
        tetromino.draw(window);
        gridDrawer.draw();

        // 显示绘制内容
        window.display();
    }
}

// 方块移动
void draw_block() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Square Movement");

    // 设置方块的边长和颜色
    float squareSize = 20.0F;
    sf::Color squareColor(255, 0, 0); // 红色

    // 创建方块
    sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
    square.setFillColor(squareColor);
    square.setOutlineThickness(1.0F);
    square.setOutlineColor(sf::Color::Black);
    square.setPosition(400.0F, 300.0F); // 初始位置

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // 处理键盘输入
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                if (square.getPosition().y - squareSize >= 0) {
                    square.move(0.0F, -squareSize);
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                if (square.getPosition().y + 2 * squareSize <= window.getSize().y) {
                    square.move(0.0F, squareSize);
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                if (square.getPosition().x - squareSize >= 0) {
                    square.move(-squareSize, 0.0F);
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                if (square.getPosition().x + 2 * squareSize <= window.getSize().x) {
                    square.move(squareSize, 0.0F);
                }
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

// void tetris() {
//     sf::RenderWindow window(sf::VideoMode(320, 480), "SFML Tetris");

//     Board board;

//     sf::Clock clock;
//     float fallTime = 0.5f;
//     float deltaTime = 0.0F;

//     while (window.isOpen()) {
//         deltaTime = clock.restart().asSeconds();

//         sf::Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed) window.close();

//             if (event.type == sf::Event::KeyPressed) {
//                 if (event.key.code == sf::Keyboard::Left)
//                     board.moveCurrentTetromino(-1, 0);
//                 else if (event.key.code == sf::Keyboard::Right)
//                     board.moveCurrentTetromino(1, 0);
//                 else if (event.key.code == sf::Keyboard::Down)
//                     board.moveCurrentTetromino(0, 1);
//                 else if (event.key.code == sf::Keyboard::Up)
//                     board.rotateCurrentTetromino();
//             }
//         }

//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
//             fallTime = 0.05f;
//         else
//             fallTime = 0.5f;

//         if (board.update(deltaTime, fallTime)) {
//             window.close();
//         }

//         window.clear(sf::Color::White);
//         board.draw(window, "images/background.png");
//         window.display();
//     }
// }

int main() {
    // tetris();
    set_window();
    // draw_block();
    return 0;
}
