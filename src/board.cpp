#include "board.hpp"

// コンストラクタ：盤面の初期化
Board::Board() {
    cells = std::vector<std::vector<Cell>>(AXIS_SIZE, std::vector<Cell>(AXIS_SIZE, Cell(Cell::EMPTY)));
    cells[0] = std::vector<Cell>(AXIS_SIZE, Cell(Cell::DOWN));
    cells[AXIS_SIZE - 1] = std::vector<Cell>(AXIS_SIZE, Cell(Cell::UP));
}

// 座標が盤面内かどうかを判定
bool Board::isOnBoard(int y, int x) {
    return (y >= 0 && y < AXIS_SIZE) && (x >= 0 && x < AXIS_SIZE);
}

// セルの状態を視覚的な文字列で表現
std::string Board::graphicCellCondition(int y, int x) const {
    Cell::CellCondition condition = cells[y][x].getCondition();
    if (condition == Cell::CellCondition::DOWN) {
        return "\\_/";
    } else if (condition == Cell::CellCondition::UP) {
        return "/^\\";
    }
    return "   ";
}

// 盤面を表示する
void Board::show() const {
    static const std::string bar(AXIS_SIZE*4, '-');
    std::cout << "   ";
    for (int i = 0; i < AXIS_SIZE; ++i) {
        std::cout << "  " << i << " ";
    }
    std::cout << "\n";

    for (int i = 0; i < AXIS_SIZE; ++i) {
        std::cout << "   -" << bar << "\n";
        std::cout << " " << i << " ";
        for (int j = 0; j < AXIS_SIZE; ++j) {
            std::cout << "|" << graphicCellCondition(i, j);
        }
        std::cout << "|\n";
    }
    std::cout << "   -" << bar << "\n";
}

void Board::changeCell(int oldY, int oldX, int newY, int newX){
    cells[newY][newX].setCondition(cells[oldY][oldX].getCondition());
    cells[oldY][oldX].setCondition(Cell::EMPTY);
}

// 指定したセルを空にする
void Board::setEmpty(int y, int x) {
    cells[y][x].setEmpty();
}

// 盤面の状態をコピー
std::vector<std::vector<Cell::CellCondition>> Board::copyState() const {
    std::vector<std::vector<Cell::CellCondition>> state(AXIS_SIZE, std::vector<Cell::CellCondition>(AXIS_SIZE));
    for (int y = 0; y < AXIS_SIZE; ++y) {
        for (int x = 0; x < AXIS_SIZE; ++x) {
            state[y][x] = cells[y][x].getCondition();
        }
    }
    return state;
}

// 盤面の状態を復元
void Board::restoreState(const std::vector<std::vector<Cell::CellCondition>>& state) {
    for (int y = 0; y < AXIS_SIZE; ++y) {
        for (int x = 0; x < AXIS_SIZE; ++x) {
            cells[y][x].setCondition(state[y][x]);
        }
    }
}


bool Board::movePiece(int oldY, int oldX, int newY, int newX, Cell::CellCondition direction) {
    if (!isOnBoard(newY, newX) || !isOnBoard(oldY, oldX)) {
        std::cout << "Invalid move: Out of bounds\n";
        return false;
    }

    if (cells[oldY][oldX].getCondition() != direction) {
        std::cout << "Invalid move: You can only move your own pieces\n";
        return false;
    }

    if (cells[newY][newX].getCondition() != Cell::EMPTY) {
        std::cout << "Invalid move: The destination cell is not empty\n";
        return false;
    }

    changeCell(oldY, oldX, newY, newX);
    capturePieces(newY, newX, direction);            // 駒を取る処理

    return true;
}

void Board::capturePieces(int y, int x, Cell::CellCondition direction) {
    Cell::CellCondition opponent = (direction == Cell::UP) ? Cell::DOWN : Cell::UP;

    captureLineCapturedPieces(y,x,direction);   // 直線上に挟まれた駒を取る処理
    // すべてのセルをチェックして捕獲処理
    for (int i = 0; i < AXIS_SIZE; ++i) {
        for (int j = 0; j < AXIS_SIZE; ++j) {
            if (cells[i][j].getCondition() != Cell::EMPTY) {
                captureTrappedPieces(i, j, cells[i][j].getCondition());
            }
        }
    }
}

void Board::captureLineCapturedPieces(int y, int x, Cell::CellCondition direction) {
    Cell::CellCondition opponent = (direction == Cell::UP) ? Cell::DOWN : Cell::UP;

    // 横方向（左から右）
    if (isOnBoard(y, x - 1) && cells[y][x - 1].getCondition() == opponent) {
        int tempX = x - 1;
        while (isOnBoard(y, tempX) && cells[y][tempX].getCondition() == opponent) {
            tempX--;
        }
        if (isOnBoard(y, tempX) && cells[y][tempX].getCondition() == direction) {
            tempX = x - 1;
            while (isOnBoard(y, tempX) && cells[y][tempX].getCondition() == opponent) {
                setEmpty(y, tempX);
                std::cout << "Captured piece at (" << y << ", " << tempX << ")\n";
                tempX--;
            }
        }
    }

    // 横方向（右から左）
    if (isOnBoard(y, x + 1) && cells[y][x + 1].getCondition() == opponent) {
        int tempX = x + 1;
        while (isOnBoard(y, tempX) && cells[y][tempX].getCondition() == opponent) {
            tempX++;
        }
        if (isOnBoard(y, tempX) && cells[y][tempX].getCondition() == direction) {
            tempX = x + 1;
            while (isOnBoard(y, tempX) && cells[y][tempX].getCondition() == opponent) {
                setEmpty(y, tempX);
                std::cout << "Captured piece at (" << y << ", " << tempX << ")\n";
                tempX++;
            }
        }
    }

    // 縦方向（上から下）
    if (isOnBoard(y - 1, x) && cells[y - 1][x].getCondition() == opponent) {
        int tempY = y - 1;
        while (isOnBoard(tempY, x) && cells[tempY][x].getCondition() == opponent) {
            tempY--;
        }
        if (isOnBoard(tempY, x) && cells[tempY][x].getCondition() == direction) {
            tempY = y - 1;
            while (isOnBoard(tempY, x) && cells[tempY][x].getCondition() == opponent) {
                setEmpty(tempY, x);
                std::cout << "Captured piece at (" << tempY << ", " << x << ")\n";
                tempY--;
            }
        }
    }

    // 縦方向（下から上）
    if (isOnBoard(y + 1, x) && cells[y + 1][x].getCondition() == opponent) {
        int tempY = y + 1;
        while (isOnBoard(tempY, x) && cells[tempY][x].getCondition() == opponent) {
            tempY++;
        }
        if (isOnBoard(tempY, x) && cells[tempY][x].getCondition() == direction) {
            tempY = y + 1;
            while (isOnBoard(tempY, x) && cells[tempY][x].getCondition() == opponent) {
                setEmpty(tempY, x);
                std::cout << "Captured piece at (" << tempY << ", " << x << ")\n";
                tempY++;
            }
        }
    }
}

bool Board::isTrapped(const std::vector<std::pair<int, int>>& region, Cell::CellCondition opponent) {
    bool is_movable[4]={false, false, false, false};    // up, down, left, right
    for (const auto& cell : region) {
        int y = cell.first, x = cell.second;
        int i=0;
        for (const auto& dir : DIRECTIONS) {
            int newY = y + dir.first;
            int newX = x + dir.second;
            if (isOnBoard(newY, newX)) {
                if (cells[newY][newX].getCondition() == Cell::EMPTY) {
                    is_movable[i]|=true; // 空のマスがあるため囲まれていない
                }
            }
            i++;
        }
    }
    bool ret=false;
    for(int i=0;i<4;i++){
        ret|=is_movable[i];
    }
    return !ret;
}

void Board::captureTrappedPieces(int y, int x, Cell::CellCondition condition){
    Cell::CellCondition target = cells[y][x].getCondition();
    Cell::CellCondition opponent = (target == Cell::UP) ? Cell::DOWN : Cell::UP;

    // BFSのためのキューと訪問済み集合
    std::queue<std::pair<int, int>> q;
    std::vector<std::pair<int, int>> region;
    std::set<std::pair<int, int>> visited;

    q.push({y, x});
    visited.insert({y, x});
    region.push_back({y, x});

    while (!q.empty()) {
        auto [y, x] = q.front();
        q.pop();

        for (const auto& dir : DIRECTIONS) {
            int newY = y + dir.first;
            int newX = x + dir.second;
            if (isOnBoard(newY, newX) && visited.find({newY, newX}) == visited.end()) {
                if (cells[newY][newX].getCondition() == target) {
                    q.push({newY, newX});
                    visited.insert({newY, newX});
                    region.push_back({newY, newX});
                }
            }
        }
    }

    // 連結領域が相手の駒または壁に囲まれているか確認
    if (isTrapped(region, opponent)) {
        for (const auto& cell : region) {
            int y = cell.first, x = cell.second;
            setEmpty(y, x);
        }
    }
}

int Board::countPlayerPiece(Cell::CellCondition condition){
    int count=0;
    for(int i=0;i<AXIS_SIZE;i++){
        for(int j=0;j<AXIS_SIZE;j++){
            if(cells[i][j].getCondition()==condition) count++;
        }
    }
    return count;
}