#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

const int M = 20; // 游戏区域的高度
const int N = 10; // 游戏区域的宽度

int field[M][N] = {0}; // 游戏区域的状态

struct Point {
    int x, y;
} a[4], b[4]; // a表示当前方块的四个点，b表示上一个状态的四个点

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
bool check() {
    for (int i = 0; i < 4; i++) {
        if (a[i].x < 0 || a[i].x >= N || a[i].y < 0 || a[i].y >= M) return false;
        else if (field[a[i].y][a[i].x]) return false;
    }
    return true;
}

void test() {
    srand(time(0));

    RenderWindow window(VideoMode(320, 480), "The Game!");

    Texture t1, t2, t3;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/background.png");
    // t3.loadFromFile("images/frame.png");

    Sprite s(t1), background(t2);
    // , frame(t3);

    int dx = 0; // 水平移动的距离
    bool rotate = false; // 是否旋转
    int colorNum = 1; // 方块的颜色
    float timer = 0, delay = 0.3; // 计时器和下落延迟

    Clock clock;

    // 初始化第一个方块
    int n = rand() % 7;
    for (int i = 0; i < 4; i++) {
        a[i].x = figures[n][i] % 2 + N / 2 - 1;
        a[i].y = figures[n][i] / 2;
    }

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Up) rotate = true;
                else if (e.key.code == Keyboard::Left) dx = -1;
                else if (e.key.code == Keyboard::Right) dx = 1;
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;

        // 水平移动
        for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].x += dx; }
        if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];

        // 旋转
        if (rotate) {
            Point p = a[1]; // 旋转中心
            for (int i = 0; i < 4; i++) {
                int x = a[i].y - p.y;
                int y = a[i].x - p.x;
                a[i].x = p.x - x;
                a[i].y = p.y + y;
            }
            if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];
        }

        // 下落
        if (timer > delay) {
            for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }

            if (!check()) {
                bool isGameOver = false;

                for (int i = 0; i < 4; i++) {
                    if (b[i].y < 0) { // 检查是否有方块到达顶部
                        isGameOver = true;
                        break;
                    }
                }

                if (isGameOver) {
                    // 游戏结束处理
                    window.close();
                } else {
                    for (int i = 0; i < 4; i++) {
                        field[b[i].y][b[i].x] = colorNum;
                    }

                    // 检查并消除满行
                    for (int i = M - 1; i >= 0; i--) {
                        bool isFull = true;
                        for (int j = 0; j < N; j++) {
                            if (field[i][j] == 0) {
                                isFull = false;
                                break;
                            }
                        }
                        if (isFull) {
                            for (int k = i; k > 0; k--) {
                                for (int j = 0; j < N; j++) {
                                    field[k][j] = field[k - 1][j];
                                }
                            }
                            i++; // 再次检查当前行
                        }
                    }

                    // 检查最高方块高度
                    int maxHeight = 0;
                    for (int i = 0; i < M; i++) {
                        for (int j = 0; j < N; j++) {
                            if (field[i][j] != 0) {
                                maxHeight = i + 1;
                                break;
                            }
                        }
                        if (maxHeight > 0) break;
                    }

                    if (maxHeight >= M) {
                        // 游戏结束处理
                        window.close();
                    } else {
                        colorNum = 1 + rand() % 7;
                        n = rand() % 7;
                        for (int i = 0; i < 4; i++) {
                            a[i].x = figures[n][i] % 2 + N / 2 - 1;
                            a[i].y = figures[n][i] / 2;
                        }
                    }
                }
            }

            timer = 0;
        }

        dx = 0;
        rotate = false;
        delay = 0.3;

        // 绘制游戏界面
        window.clear(Color::White);
        window.draw(background);

        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (field[i][j] == 0) continue;
                s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
                s.setPosition(j * 18, i * 18);
                s.move(28, 31); // offset
                window.draw(s);
            }
        }

        for (int i = 0; i < 4; i++) {
            s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
            s.setPosition(a[i].x * 18, a[i].y * 18);
            s.move(28, 31); // offset
            window.draw(s);
        }

        // window.draw(frame);
        window.display();
    }
}
