/**
 * @Author: fengfuyuyang@outlook.com
 * @File Name: block.cpp
 * @File Version: 0.1
 * @Create Time: 2024-07-17 11:23:07
 * 
 * Copyright (c) 2024 fengfuyuyang@outlook.com. All rights reserved.
 */

#include "block.h"
#include "background.h"

int field[kGameHeight][kGameWidth] = {0}; // 游戏区域的状态

Block::Block(const std::wstring& filename) : _is_paused(false) {
    std::wstring path = FileHandler::GetFullPath(filename);
    std::string texture_path = FileHandler::ConvertToString(path);
    if (!FileHandler::FileExists(path)) {
        // 处理文件不存在的情况
        throw std::runtime_error("File not found: " + texture_path);
    }

    // 加载纹理
    if (!_texture.loadFromFile(texture_path)) {
        // 处理加载失败的情况
        throw std::runtime_error("Failed to load texture from " + texture_path);
    }
    CreateSprites();
    InitRandomEngine();
}

void Block::GetNewBlock() {
    std::uniform_int_distribution<int> block_type_dist(0, kTypeSize - 1);
    std::uniform_int_distribution<int> sprite_dist(0, _sprites.size() - 1);
    std::uniform_int_distribution<int> rotation_dist(0, 3);

    _type = static_cast<BlockType>(block_type_dist(_random_engine));
    int current_sprite = sprite_dist(_random_engine);

    for (int i = 0; i < kBlockNum; i++) {
        _current_block[i].x = _figures[_type][i] % 2 * -1;
        _current_block[i].y = (_figures[_type][i] + 1) / 2 - kBlockNum;
        _current_block[i].sprite = _sprites[current_sprite];
    }

    // 起始方块旋转
    int rotation = rotation_dist(_random_engine);
    for (int i = 0; i < rotation; i++) {
        Rotate();
    }

    // 矫正位置居中
    int min_x = kGameWidth, max_x = 0;
    for (int i = 0; i < kBlockNum; i++) {
        if (_current_block[i].x < min_x) {
            min_x = _current_block[i].x;
        }
        if (_current_block[i].x > max_x) {
            max_x = _current_block[i].x;
        }
    }
    int offset_x = (kGameWidth / 2) - ((max_x - min_x + 1) / 2 + min_x);
    for (int i = 0; i < kBlockNum; i++) {
        _current_block[i].x += offset_x;
    }
}

void Block::Draw(sf::RenderWindow& window) {
    for (auto& it : _points) {
        it.sprite.setPosition(it.x * GetSideLength(), it.y * GetSideLength());
        it.sprite.move(X, Y);
        window.draw(it.sprite);
    }
    for (auto& it : _current_block) {
        it.sprite.setPosition(it.x * GetSideLength(), it.y * GetSideLength());
        it.sprite.move(X, Y);
        if (it.y >= 0) {
            window.draw(it.sprite);
        }
    }
}

// 检查方块是否在有效区域内
bool Block::IsValidPosition() {
    for (auto& it : _current_block) {
        if (it.x < 0 || it.x >= kGameWidth || it.y >= kGameHeight) {
            return false;
        } else if (field[it.y][it.x]) {
            return false;
        }
    }
    return true;
}

// 旋转方块
void Block::Rotate() {
    if (_is_paused) {
        return;
    }

    CopyCurrentToPrevious();
    switch (_type) {
    case BlockType::I: {
        if (_current_block[1].x != _current_block[2].x) {
            RotatePointNeg90(2);
        } else {
            RotatePoint90(2);
        }

        break;
    }
    case BlockType::J: {
        if (_current_block[3].x > _current_block[2].x) {
            Move(1, 0);
        }
        RotatePoint90(1);
        if (_current_block[3].x > _current_block[2].x) {
            Move(-1, 0);
        }
        break;
    }
    case BlockType::L: {
        if (_current_block[2].x > _current_block[3].x) {
            Move(-1, 0);
        }
        RotatePoint90(1);
        if (_current_block[2].x > _current_block[3].x) {
            Move(1, 0);
        }
        break;
    }
    case BlockType::O:
        break;
    case BlockType::S: {
        if (_current_block[1].x != _current_block[2].x) {
            RotatePointNeg90(1);
        } else {
            RotatePoint90(1);
        }

        break;
    }
    case BlockType::T: {
        RotatePoint90(1);
        break;
    }
    case BlockType::Z: {
        if (_current_block[1].x != _current_block[2].x) {
            RotatePoint90(1);
        } else {
            RotatePointNeg90(1);
        }

        break;
    }
    default:
        break;
    }

    if (!IsValidPosition()) {
        CopyPreviousToCurrent();
    }
}

// 移动方块
void Block::Move(int dx, int dy) {
    if (_is_paused) {
        return;
    }

    CopyCurrentToPrevious();
    for (int i = 0; i < kBlockNum; i++) {
        _current_block[i].x += dx;
        _current_block[i].y += dy;
    }
    if (!IsValidPosition()) {
        CopyPreviousToCurrent();
    }
}

// 下落方块
void Block::Drop() {
    if (_is_paused) {
        return;
    }
    CopyCurrentToPrevious();
    for (auto& it : _current_block) {
        it.y += 1;
    }
    if (!IsValidPosition()) {
        CopyPreviousToCurrent();
        FixBlock();
    }
}

void Block::DropToBottom() {
    if (_is_paused) {
        return;
    }

    while (true) {
        CopyCurrentToPrevious();
        for (auto& it : _current_block) {
            it.y += 1;
        }
        if (!IsValidPosition()) {
            CopyPreviousToCurrent();
            FixBlock();
            break;
        }
    }
}

bool Block::IsGameOver() {
    bool is_game_over = false;
    CopyCurrentToPrevious();
    for (auto& it : _current_block) {
        it.y += 1;
    }
    if (!IsValidPosition()) {
        for (auto& it : _current_block) {
            if (it.y <= 0) {
                is_game_over = true;
                break;
            }
        }
    }
    CopyPreviousToCurrent();
    return is_game_over;
}

void Block::ChangePauseStatus() {
    _is_paused = !_is_paused;
}

// 获取正方形的边长
int Block::GetSideLength() const {
    return _texture.getSize().y;
}

// 获取正方形的数量
int Block::GetSquareCount() const {
    return _texture.getSize().x / GetSideLength();
}

// 创建所有正方形的Sprite
void Block::CreateSprites() {
    // 遍历所有正方形的编号
    for (int i = 0; i < GetSquareCount(); ++i) {
        // 创建Sprite并设置纹理
        sf::Sprite sprite(_texture);

        // 设置纹理矩形, 左上角坐标为(i * GetSideLength(), 0), 大小为(GetSideLength(), GetSideLength())
        sprite.setTextureRect(
                sf::IntRect(i * GetSideLength(), 0, GetSideLength(), GetSideLength()));
        _sprites.push_back(sprite);
    }
}

void Block::RotatePoint90(int pos) {
    Point center = _current_block[pos]; // 旋转中心
    for (auto& it : _current_block) {
        int x = it.y - center.y;
        int y = it.x - center.x;
        it.x = center.x - x;
        it.y = center.y + y;
    }
}

void Block::RotatePointNeg90(int pos) {
    Point center = _current_block[pos]; // 旋转中心
    for (auto& it : _current_block) {
        int x = it.y - center.y;
        int y = it.x - center.x;
        it.x = center.x + x;
        it.y = center.y - y;
    }
}

void Block::CopyCurrentToPrevious() {
    for (int i = 0; i < kBlockNum; ++i) {
        _previous_block[i] = _current_block[i];
    }
}

void Block::CopyPreviousToCurrent() {
    for (int i = 0; i < kBlockNum; ++i) {
        _current_block[i] = _previous_block[i];
    }
}

void Block::FixBlock() {
    for (auto& it : _current_block) {
        // if (it.y >= 0) {
        field[it.y][it.x] = 1;
        _points.push_back(it);
        // }
    }
    ClearLines();
    GetNewBlock();
}

void Block::ClearLines() {
    int num_lines_cleared = 0;

    for (int y = kGameHeight - 1; y >= 0; --y) {
        bool is_line_full = true;

        for (int x = 0; x < kGameWidth; ++x) {
            if (field[y][x] == 0) {
                is_line_full = false;
                break;
            }
        }

        if (is_line_full) {
            num_lines_cleared++;
            for (int x = 0; x < kGameWidth; ++x) {
                field[y][x] = 0;
            }

            // 使用remove_if和erase移除当前行的所有方块
            _points.remove_if([y](const Point& point) { return point.y == y; });

            // 将上方的所有方块向下移动一行
            for (auto& point : _points) {
                if (point.y < y) {
                    point.y += 1;
                }
            }

            // 将上方的所有行向下移动一行
            for (int row = y; row > 0; --row) {
                for (int col = 0; col < kGameWidth; ++col) {
                    field[row][col] = field[row - 1][col];
                }
            }

            // 重新检查当前行
            y++;
        }
    }
}

void Block::InitRandomEngine() {
    std::random_device rd;
    _random_engine = std::mt19937(rd());
}
