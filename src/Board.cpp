#include "Board.h"

#include <cstring>

Board::Board() {
    memset(board, 0, sizeof(board));
    currentTetromino.reset();
    currentTime = 0.0f;
}

bool Board::update(float deltaTime, float fallTime) {
    currentTime += deltaTime;
    if (currentTime > fallTime) {
        if (!currentTetromino.canMove(0, 1, board)) {
            const int(*position)[2] = currentTetromino.getPosition();
            sf::Color color = currentTetromino.getColor();

            for (int i = 0; i < 4; ++i) {
                int x = position[i][0];
                int y = position[i][1];
                board[x][y] = sf::Vector2i(color.toInteger(), 1);
            }

            clearLines();
            currentTetromino.reset();

            if (!currentTetromino.canMove(0, 0, board)) return true; // Game over
        } else {
            currentTetromino.move(0, 1);
        }
        currentTime = 0.0f;
    }
    return false;
}

void Board::draw(sf::RenderWindow& window, const std::string& background_image_path) {
    sf::Texture texture;
    if (texture.loadFromFile(background_image_path)) {
        sf::Sprite background(texture);
        window.draw(background);
    }

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 20; ++j) {
            if (board[i][j] != sf::Vector2i(0, 0)) {
                sf::RectangleShape shape(sf::Vector2f(32, 32));
                shape.setPosition(i * 32, j * 32);
                shape.setFillColor(sf::Color(board[i][j].x));
                window.draw(shape);
            }
        }
    }
    currentTetromino.draw(window);
}

void Board::moveCurrentTetromino(int x, int y) {
    if (currentTetromino.canMove(x, y, board)) currentTetromino.move(x, y);
}

void Board::rotateCurrentTetromino() {
    currentTetromino.rotate(board);
}

void Board::clearLines() {
    for (int j = 0; j < 20; ++j) {
        bool isFull = true;
        for (int i = 0; i < 10; ++i) {
            if (board[i][j] == sf::Vector2i(0, 0)) {
                isFull = false;
                break;
            }
        }
        if (isFull) {
            for (int y = j; y > 0; --y) {
                for (int x = 0; x < 10; ++x) {
                    board[x][y] = board[x][y - 1];
                }
            }
            for (int x = 0; x < 10; ++x) {
                board[x][0] = sf::Vector2i(0, 0);
            }
        }
    }
}
