#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <chrono>

#include "background.h"
#include "square.h"
#include "tetromino.h"

// 随机生成一个新的方块
Tetromino* RandomTetromino(Square& square, const sf::FloatRect& bounds) {
    std::random_device rd;
    unsigned int seed = rd();

    // 使用时间戳进一步混合种子
    seed ^= static_cast<unsigned int>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dis(0, 6);
    int random = dis(gen);
    switch (random) {
    case 0:
        return new I_Tetromino(square, bounds);
    case 1:
        return new J_Tetromino(square, bounds);
    case 2:
        return new L_Tetromino(square, bounds);
    case 3:
        return new O_Tetromino(square, bounds);
    case 4:
        return new S_Tetromino(square, bounds);
    case 5:
        return new T_Tetromino(square, bounds);
    case 6:
        return new Z_Tetromino(square, bounds);
    default:
        return nullptr;
    }
}

void start() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    Square square(L"images\\tetromino.png");
    int side_length = square.GetSideLength();
    Background background(L"images\\background.png", side_length);
    int width = background.GetSize().x;
    int height = background.GetSize().y;
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML Tetris");

    Tetromino* tetromino = RandomTetromino(square, background.GetBounds());
    // 创建时钟
    sf::Clock clock;
    sf::Time timeSinceLastMove = sf::Time::Zero;
    sf::Time timePerMove = sf::seconds(0.5F);
    bool game_over = false;
    std::vector<Tetromino*> tetrominoes;

    tetrominoes.push_back(tetromino);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (!game_over && event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    tetromino->Rotate();
                } else if (event.key.code == sf::Keyboard::Down) {
                    tetromino->Move(0, 1);
                } else if (event.key.code == sf::Keyboard::Left) {
                    tetromino->Move(-1, 0);
                } else if (event.key.code == sf::Keyboard::Right) {
                    tetromino->Move(1, 0);
                }
            }
        }

        // if (!game_over) {
            // 更新方块自动下落的计时器
            // timeSinceLastMove += clock.restart();
            // if (timeSinceLastMove >= timePerMove) {
            //     tetromino->Move(0, 1);
            //     timeSinceLastMove = sf::Time::Zero;
            // }


        // }
        if (!tetromino->CanMove(0, 1)) {
                // tetromino->FixToBackground(background);

                // background.CheckLines();

                // delete tetromino;
                tetromino = RandomTetromino(square, background.GetBounds());
                tetrominoes.push_back(tetromino);
            }
        window.clear(sf::Color::White);
        background.Draw(window);

        for (auto& t : tetrominoes) {
            t->Draw(window);
        }

        // if (game_over) {
        //     sf::Font font;
        //     font.loadFromFile("arial.ttf");
        //     sf::Text text("Game Over!", font, 50);
        //     text.setFillColor(sf::Color::Red);
        //     text.setPosition(width / 2 - text.getLocalBounds().width / 2,
        //                      height / 2 - text.getLocalBounds().height / 2);
        //     window.draw(text);
        // }

        window.display();
    }

    delete tetromino;
}

int main() {
    start();
    return 0;
}
