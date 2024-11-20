#include "hasamishogi_AI.hpp"
#include <limits>

HasamiShogiAI::HasamiShogiAI(Cell::CellCondition my_piece):my_piece(my_piece) {}

// 評価関数: 駒数の差を基本とする
int HasamiShogiAI::evaluate(const std::vector<std::vector<Cell::CellCondition>>& boardState) {
    int score = 0;
    Cell::CellCondition OPPONENT=(my_piece==Cell::CellCondition::DOWN ? Cell::CellCondition::UP : Cell::CellCondition::DOWN);
    for (size_t y = 0; y < boardState.size(); ++y) {
        for (size_t x = 0; x < boardState[0].size(); ++x) {
            if (boardState[y][x] == Cell::CellCondition::UP) {
                score += 10; // 自分の駒
            } else if (boardState[y][x] == OPPONENT) {
                score -= 10; // 相手の駒
            }
        }
    }

    // 追加の評価要素を計算
    score += evaluateCapturePotential(boardState, my_piece) * 20;  // 挟める可能性
    score -= evaluateCapturePotential(boardState, OPPONENT) * 15; // 相手に挟まれる可能性

    return score;
}

// 挟める駒の数を計算
int HasamiShogiAI::evaluateCapturePotential(const std::vector<std::vector<Cell::CellCondition>>& boardState, Cell::CellCondition player) {
    int capturePotential = 0;

    for (size_t y = 0; y < boardState.size(); ++y) {
        for (size_t x = 0; x < boardState[0].size(); ++x) {
            if (boardState[y][x] == player) {
                for (const auto& dir : DIRECTIONS) {
                    int targetY = y + dir.first;
                    int targetX = x + dir.second;
                    int beyondY = targetY + dir.first;
                    int beyondX = targetX + dir.second;

                    if (Board::isOnBoard(targetY, targetX) && Board::isOnBoard(beyondY, beyondX)) {
                        if (boardState[targetY][targetX] == -player && boardState[beyondY][beyondX] == player) {
                            capturePotential++;
                        }
                    }
                }
            }
        }
    }

    return capturePotential;
}

// 最善手を探索
std::pair<std::pair<int, int>, std::pair<int, int>> HasamiShogiAI::findBestMove(Board& board) {
    Cell::CellCondition PLAYER=my_piece;
    Cell::CellCondition OPPONENT=(my_piece==Cell::CellCondition::DOWN ? Cell::CellCondition::UP : Cell::CellCondition::DOWN);
    std::vector<std::vector<Cell::CellCondition>> state = board.copyState();
    int bestScore = std::numeric_limits<int>::min();
    std::pair<std::pair<int, int>, std::pair<int, int>> bestMove;

    for (size_t y = 0; y < state.size(); ++y) {
        for (size_t x = 0; x < state[0].size(); ++x) {
            if (state[y][x] == PLAYER) {
                for (const auto& dir : DIRECTIONS) {
                    int newY = y + dir.first;
                    int newX = x + dir.second;

                    // 指定方向に駒を移動可能な限り試す
                    while (Board::isOnBoard(newY, newX) && state[newY][newX] == Cell::EMPTY) {
                        // 仮想的に移動して評価
                        std::vector<std::vector<Cell::CellCondition>> tempState = state;
                        tempState[newY][newX] = PLAYER;
                        tempState[y][x] = Cell::EMPTY;

                        int score = minimax(tempState, 3, false); // 深さ3で探索
                        if (score > bestScore) {
                            bestScore = score;
                            bestMove = {{y, x}, {newY, newX}};
                        }

                        // 次のマスへ進む
                        newY += dir.first;
                        newX += dir.second;
                    }
                }
            }
        }
    }

    return bestMove;
}

// ミニマックス法で数手先を読む
int HasamiShogiAI::minimax(std::vector<std::vector<Cell::CellCondition>>& boardState, int depth, bool isMaximizing) {
    if (depth == 0) {
        return evaluate(boardState);
    }

    int bestScore = isMaximizing ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();

    Cell::CellCondition PLAYER=my_piece;
    Cell::CellCondition OPPONENT=(my_piece==Cell::CellCondition::DOWN ? Cell::CellCondition::UP : Cell::CellCondition::DOWN);
    Cell::CellCondition EMPTY=Cell::EMPTY;

    for (size_t y = 0; y < boardState.size(); ++y) {
        for (size_t x = 0; x < boardState[0].size(); ++x) {
            if ((isMaximizing && boardState[y][x] == PLAYER) || (!isMaximizing && boardState[y][x] == OPPONENT)) {
                for (const auto& dir : DIRECTIONS) {
                    int newY = y + dir.first;
                    int newX = x + dir.second;

                    while (Board::isOnBoard(newY, newX) && boardState[newY][newX] == EMPTY) {
                        std::vector<std::vector<Cell::CellCondition>> tempState = boardState;
                        tempState[newY][newX] = boardState[y][x];
                        tempState[y][x] = EMPTY;

                        int score = minimax(tempState, depth - 1, !isMaximizing);
                        bestScore = isMaximizing ? std::max(bestScore, score) : std::min(bestScore, score);

                        newY += dir.first;
                        newX += dir.second;
                    }
                }
            }
        }
    }

    return bestScore;
}