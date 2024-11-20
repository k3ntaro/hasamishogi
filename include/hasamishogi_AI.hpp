#ifndef HASAMISHOGI_AI_HPP
#define HASAMISHOGI_AI_HPP

#include "board.hpp"
#include <vector>
#include <utility>

class HasamiShogiAI {
public:
    HasamiShogiAI(Cell::CellCondition my_piece);

    Cell::CellCondition my_piece;

    // 最善手を探索する
    std::pair<std::pair<int, int>, std::pair<int, int>> findBestMove(Board& board);

private:
    const std::vector<std::pair<int, int>> DIRECTIONS = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    // 評価関数
    int evaluate(const std::vector<std::vector<Cell::CellCondition>>& boardState);
    int evaluateCapturePotential(const std::vector<std::vector<Cell::CellCondition>>& boardState, Cell::CellCondition player);
    int minimax(std::vector<std::vector<Cell::CellCondition>>& boardState, int depth, bool isMaximizing);
};

#endif // HASAMISHOGI_AI_HPP
