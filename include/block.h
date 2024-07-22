/**
 * @Author: fengfuyuyang@outlook.com
 * @File Name: block.h
 * @File Version: 0.1
 * @Create Time: 2024-07-17 11:13:41
 * 
 * Copyright (c) 2024 fengfuyuyang@outlook.com. All rights reserved.
 */

#pragma once

#include <SFML/Graphics.hpp>
#include <list>
#include <random>
#include <vector>

const int kTypeSize = 7;    // 方块种类个数
const int kBlockNum = 4;    // 方块的数量

class Block {
public:
    explicit Block(const std::wstring& filename);

    void GetNewBlock();

    void Draw(sf::RenderWindow& window);

    // 检查方块是否在有效区域内
    bool IsValidPosition();

    // 旋转方块
    void Rotate();

    // 移动方块
    void Move(int dx, int dy);

    // 下落方块
    void Drop();

    void DropToBottom();

    bool IsGameOver();

    void ChangePauseStatus();

    void Reset();

private:
    struct Point {
        int x;
        int y;
        sf::Sprite sprite;
    };

    Point _current_block[kBlockNum];  // _current_block表示当前方块的四个点
    Point _previous_block[kBlockNum]; //_previous_block表示上一个状态的四个点
    int _figures[kTypeSize][kBlockNum] = {
            1, 3, 5, 7, // I
            4, 6, 8, 7, // J
            3, 5, 7, 8, // L
            5, 7, 6, 8, // O
            3, 5, 6, 8, // S
            3, 5, 7, 6, // T
            4, 6, 5, 7  // Z
    };

    enum BlockType { I = 0, J, L, O, S, T, Z };

    // 获取正方形的边长
    int GetSideLength() const;

    // 获取正方形的数量
    int GetSquareCount() const;

    // 创建所有正方形的Sprite
    void CreateSprites();

    // 以pos为中心顺时针旋转90度
    void RotatePoint90(int pos);

    // 以pos为中心逆时针旋转90度
    void RotatePointNeg90(int pos);

    void CopyCurrentToPrevious();

    void CopyPreviousToCurrent();

    void FixBlock();

    // 清理满行
    void ClearLines();

    void InitRandomEngine();

    // 纹理
    sf::Texture _texture;
    // 所有正方形的Sprite
    std::vector<sf::Sprite> _sprites;
    // 当前方块类型
    BlockType _type;
    std::list<Point> _points;
    std::mt19937 _random_engine;
    // 游戏是否处于暂停状态
    bool _is_paused;
};
