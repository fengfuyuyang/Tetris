#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

#include "file_handler.h"

const int kGameHeight = 20; // 游戏区域的高度
const int kGameWidth = 10;  // 游戏区域的宽度
const int kTypeSize = 7;    // 方块种类个数
const int kBlockNum = 4;

int field[kGameHeight][kGameWidth] = {0}; // 游戏区域的状态

class Block {
public:
    explicit Block(const std::wstring& filename) {
        std::wstring path = FileHandler::GetFullPath(filename);
        std::string texture_path = FileHandler::ConvertToString(path);
        if (!FileHandler::FileExists(path)) {
            // 处理文件不存在的情况
            throw std::runtime_error("File not found: " + texture_path);
        }

        // 加载纹理
        if (!m_texture.loadFromFile(texture_path)) {
            // 处理加载失败的情况
            throw std::runtime_error("Failed to load texture from " + texture_path);
        }
        CreateSprites();
    }

    // 获取正方形的数量
    int GetSquareCount() const { return m_texture.getSize().x / GetSideLength(); }

    void Init() {
        _type = static_cast<BlockType>(rand() % kTypeSize);
        _current_sprite = rand() % _sprites.size();

        for (int i = 0; i < kBlockNum; i++) {
            _current_block[i].x = _figures[_type][i] % 2 * -1 + kGameWidth / 2;

            // 使得方块从底部开始落下, 依次进入游戏区域
            _current_block[i].y = (_figures[_type][i] + 1) / 2 - kBlockNum;
        }
    }

    void Draw(sf::RenderWindow& window) {
        for (auto& it : _current_block) {
            _sprites[_current_sprite].setPosition(it.x * GetSideLength(), it.y * GetSideLength());
            _sprites[_current_sprite].move(28, 31);
            if (it.y >= 0) {
                window.draw(_sprites[_current_sprite]);
            }
        }
    }

    // 检查方块是否在有效区域内
    bool IsValidPosition() {
        for (auto& it : _current_block) {
            if (it.x < 0 || it.x >= kGameWidth || it.y >= kGameHeight) {
                return false;
            } /*else if (field[it.y][it.x]) {
                return false;
            }*/
        }
        return true;
    }

    // 旋转方块
    void Rotate() {
        switch (_type) {
        case BlockType::I: {
            if (_current_block[1].x != _current_block[2].x) {
                RotatePointNeg90(1);
            } else {
                RotatePoint90(1);
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
            RotatePoint90(1);
            break;
        }

        if (!IsValidPosition()) {
            for (int i = 0; i < 4; i++) {
                _current_block[i] = _previous_block[i];
            }
        }
    }

    // 移动方块
    void Move(int dx, int dy) {
        for (int i = 0; i < 4; i++) {
            _previous_block[i] = _current_block[i];
            _current_block[i].x += dx;
            _current_block[i].y += dy;
        }
        if (!IsValidPosition()) {
            for (int i = 0; i < 4; i++) {
                _current_block[i] = _previous_block[i];
            }
        }
    }

    // // 下落方块
    // void Drop() {
    //     for (int i = 0; i < 4; i++) {
    //         _previous_block[i] = _current_block[i];
    //         _current_block[i].y += 1;
    //     }
    //     if (!IsValidPosition()) {
    //         for (int i = 0; i < 4; i++) {
    //             _current_block[i] = _previous_block[i];
    //         }
    //     }
    // }

    // // 获取方块位置信息
    // Point* GetBlocks() { return _current_block; }

private:
    struct Point {
        int x, y;
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
    int GetSideLength() const { return m_texture.getSize().y; }

    // 创建所有正方形的Sprite
    void CreateSprites() {
        // 遍历所有正方形的编号
        for (int i = 0; i < GetSquareCount(); ++i) {
            // 创建Sprite并设置纹理
            sf::Sprite sprite(m_texture);

            // 设置纹理矩形, 左上角坐标为(i * GetSideLength(), 0), 大小为(GetSideLength(), GetSideLength())
            sprite.setTextureRect(
                    sf::IntRect(i * GetSideLength(), 0, GetSideLength(), GetSideLength()));
            _sprites.push_back(sprite);
        }
    }

    void RotatePoint90(int pos) {
        Point center = _current_block[pos]; // 旋转中心
        for (auto& it : _current_block) {
            int x = it.y - center.y;
            int y = it.x - center.x;
            it.x = center.x - x;
            it.y = center.y + y;
        }
    }

    void RotatePointNeg90(int pos) {
        Point center = _current_block[pos]; // 旋转中心
        for (auto& it : _current_block) {
            int x = it.y - center.y;
            int y = it.x - center.x;
            it.x = center.x + x;
            it.y = center.y - y;
        }
    }

    // 纹理
    sf::Texture m_texture;
    // 所有正方形的Sprite
    std::vector<sf::Sprite> _sprites;
    // 当前方块类型
    BlockType _type;
    int _current_sprite;
};
