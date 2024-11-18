#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "cell.hpp"
#include "board.hpp"
#include <string>

class Player {
public:
    // コンストラクタ
    Player(Cell::CellCondition direction, bool isComputer = false);

    // プレイヤーの駒の向きを取得
    std::string getDirectionStr() const;

    // プレイヤーの所持駒数を取得
    int getNumPieces() const { return numPieces; }

    // プレイヤーの捕獲した駒の数を取得
    int getCaptureCount() const { return captureCount; }

    // プレイヤーがコンピュータかどうかを確認
    bool isComputer() const { return isComputerPlayer; }

private:
    Cell::CellCondition direction;          // プレイヤーの駒の向き (UP / DOWN)
    int numPieces;          // 所持している駒の数
    int captureCount;       // 取った駒の数
    bool isComputerPlayer;  // プレイヤーがコンピュータかどうか
    std::string strDirection; // 駒の向きの文字列
};

#endif // PLAYER_HPP
