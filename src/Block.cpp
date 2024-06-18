#include "Block.h"

Block::Block() {
    // 初始化方块
}

bool Block::canMove(int x, int y) {
    // 检查方块是否可以移动到新的位置
    return true;
}

void Block::move(int x, int y) {
    // 移动方块到新的位置
}

void Block::rotate() {
    // 旋转方块
}

void Block::draw(sf::RenderWindow &window) {
    // 绘制方块
}

void Block::setColor(sf::Color color) {
    shape.setFillColor(color);
}
