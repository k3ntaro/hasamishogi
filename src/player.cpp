#include "player.hpp"
#include "cell.hpp"
#include "board.hpp"
#include <string>

// コンストラクタ
Player::Player(Cell::CellCondition direction, bool isComputer) 
    : direction(direction), numPieces(Board::AXIS_SIZE), captureCount(0), isComputerPlayer(isComputer) {
    // 駒の向きの文字列を設定
    if (direction == Cell::CellCondition::UP) {
        strDirection = "UP";
    } else if (direction == Cell::CellCondition::DOWN) {
        strDirection = "DOWN";
    }
}

// プレイヤーの駒の向きを取得
std::string Player::getDirectionStr() const {
    return strDirection;
}
