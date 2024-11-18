#ifndef BOARD_HPP
#define BOARD_HPP

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <queue>
#include "cell.hpp"

class Board {
public:
    static const int AXIS_SIZE = 9;
    const std::vector<std::pair<int, int>> DIRECTIONS = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

private:
    std::vector<std::vector<Cell>> cells;

    // 駒の状態を変える
    void changeCell(int oldY, int oldX, int newY, int newX);

    // 駒を取る処理（上下左右に挟まれている駒を取る）
    void capturePieces(int y, int x, Cell::CellCondition direction);

    // 直線上に並んでいる駒を取る処理（オセロのように挟んだ場合）
    void captureLineCapturedPieces(int y, int x, Cell::CellCondition direction);

    // 壁と駒によって動けない駒を取る処理
    bool isTrapped(const std::vector<std::pair<int, int>>& region, Cell::CellCondition opponent);
    void captureTrappedPieces(int y, int x, Cell::CellCondition direction);

    // セルを空にする
    void setEmpty(int y, int x);

public:
    // コンストラクタ
    Board();

    // 座標が盤面内かどうかを判定
    static bool isOnBoard(int y, int x);

    // セルの状態を視覚的な文字列で表現
    std::string graphicCellCondition(int y, int x) const;

    // 盤面を表示する
    void show() const;

    // 駒を移動させる
    bool movePiece(int oldY, int oldX, int newY, int newX, Cell::CellCondition direction);

    // プレイヤーの駒を数える
    int countPlayerPiece(Cell::CellCondition conditon);

    // 盤面の状態をコピー
    std::vector<std::vector<Cell::CellCondition>> copyState() const;

    // 盤面の状態を復元
    void restoreState(const std::vector<std::vector<Cell::CellCondition>>& state);
};

#endif // BOARD_HPP
