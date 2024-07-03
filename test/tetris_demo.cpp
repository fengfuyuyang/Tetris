#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

using namespace sf;

const int kGameHeight = 20; // 游戏区域的高度
const int kGameWidth = 10;  // 游戏区域的宽度

int field[kGameHeight][kGameWidth] = {0}; // 游戏区域的状态

struct Point {
    int x, y;
} currentBlock[4],
        previousBlock[4]; // currentBlock表示当前方块的四个点，previousBlock表示上一个状态的四个点

int figures[7][4] = {
        1, 3, 5, 7, // I
        2, 4, 5, 7, // Z
        3, 5, 4, 6, // S
        3, 5, 4, 7, // T
        2, 3, 5, 7, // L
        3, 5, 7, 6, // J
        2, 3, 4, 5  // O
};

// 检查方块是否在有效区域内
bool isValidPosition() {
    for (int i = 0; i < 4; i++) {
        if (currentBlock[i].x < 0 || currentBlock[i].x >= kGameWidth || currentBlock[i].y < 0 ||
            currentBlock[i].y >= kGameHeight)
            return false;
        else if (field[currentBlock[i].y][currentBlock[i].x])
            return false;
    }
    return true;
}

// 旋转方块
void rotateBlock() {
    Point center = currentBlock[1]; // 旋转中心
    for (int i = 0; i < 4; i++) {
        int x = currentBlock[i].y - center.y;
        int y = currentBlock[i].x - center.x;
        currentBlock[i].x = center.x - x;
        currentBlock[i].y = center.y + y;
    }
    if (!isValidPosition()) {
        for (int i = 0; i < 4; i++) currentBlock[i] = previousBlock[i];
    }
}

// 移动方块
void moveBlock(int dx) {
    for (int i = 0; i < 4; i++) {
        previousBlock[i] = currentBlock[i];
        currentBlock[i].x += dx;
    }
    if (!isValidPosition()) {
        for (int i = 0; i < 4; i++) currentBlock[i] = previousBlock[i];
    }
}

// 下落方块
void dropBlock() {
    for (int i = 0; i < 4; i++) {
        previousBlock[i] = currentBlock[i];
        currentBlock[i].y += 1;
    }
    if (!isValidPosition()) {
        bool gameOver = false;
        for (int i = 0; i < 4; i++) {
            if (previousBlock[i].y < 0) {
                gameOver = true;
                break;
            }
        }
        if (gameOver) {
            // 处理游戏结束
            // window.close();
            return;
        } else {
            for (int i = 0; i < 4; i++) {
                field[previousBlock[i].y][previousBlock[i].x] = 1; // 设置方块的颜色
            }

            // 检查并消除满行
            for (int i = kGameHeight - 1; i >= 0; i--) {
                bool isFull = true;
                for (int j = 0; j < kGameWidth; j++) {
                    if (field[i][j] == 0) {
                        isFull = false;
                        break;
                    }
                }
                if (isFull) {
                    for (int k = i; k > 0; k--) {
                        for (int j = 0; j < kGameWidth; j++) {
                            field[k][j] = field[k - 1][j];
                        }
                    }
                    i++; // 再次检查当前行
                }
            }

            // 检查最高方块高度
            int maxHeight = 0;
            for (int i = 0; i < kGameHeight; i++) {
                for (int j = 0; j < kGameWidth; j++) {
                    if (field[i][j] != 0) {
                        maxHeight = i + 1;
                        break;
                    }
                }
                if (maxHeight > 0) break;
            }

            if (maxHeight >= kGameHeight) {
                // 处理游戏结束
                // window.close();
                return;
            } else {
                // 生成新的方块
                int nextBlockType = rand() % 7;
                for (int i = 0; i < 4; i++) {
                    currentBlock[i].x = figures[nextBlockType][i] % 2 + kGameWidth / 2 - 1;
                    currentBlock[i].y = figures[nextBlockType][i] / 2;
                }
            }
        }
    }
}

void renderGame(RenderWindow& window) {
    // 清空窗口
    window.clear(Color::White);

    // 绘制游戏区域
    for (int i = 0; i < kGameHeight; i++) {
        for (int j = 0; j < kGameWidth; j++) {
            if (field[i][j] == 0) continue;
            // 绘制方块
        }
    }

    // 绘制当前方块
    for (int i = 0; i < 4; i++) {
        // 绘制方块
    }

    // 显示窗口
    window.display();
}

void runGame() {
    srand(time(nullptr));

    RenderWindow window(VideoMode(320, 480), "Tetris");

    Clock clock;
    float timer = 0, delay = 0.3;
    bool rotate = false;
    int dx = 0;

    int currentBlockType = rand() % 7;
    for (int i = 0; i < 4; i++) {
        currentBlock[i].x = figures[currentBlockType][i] % 2 + kGameWidth / 2 - 1;
        currentBlock[i].y = figures[currentBlockType][i] / 2;
    }

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        timer += deltaTime;

        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();
            else if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Up)
                    rotate = true;
                else if (e.key.code == Keyboard::Left)
                    dx = -1;
                else if (e.key.code == Keyboard::Right)
                    dx = 1;
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;

        // 移动方块
        moveBlock(dx);

        // 旋转方块
        if (rotate) {
            rotateBlock();
        }

        // 下落方块
        if (timer > delay) {
            dropBlock();
            timer = 0;
        }

        dx = 0;
        rotate = false;
        delay = 0.3;

        // 渲染游戏界面
        renderGame(window);
    }
}

int main() {
    runGame();

    return 0;
}
