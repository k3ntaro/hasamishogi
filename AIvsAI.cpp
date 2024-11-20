#include "board.hpp"
#include "hasamishogi_AI.hpp"
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

// AIのターンを処理
bool aiTurn(Board& board, HasamiShogiAI& ai, int player) {
    board.show();
    cout << "AI (" << (player == 1 ? "1" : "2") << ") の番です...\n";

    auto move = ai.findBestMove(board);
    int oldY = move.first.first;
    int oldX = move.first.second;
    int newY = move.second.first;
    int newX = move.second.second;

    cout << "AI (" << (player == 1 ? "1" : "2") << ") が (" << oldY << ", " << oldX << ") から (" 
         << newY << ", " << newX << ") に移動しました。\n";

    // 駒の移動
    board.movePiece(oldY, oldX, newY, newX, ai.my_piece);
    return true;
}

int main() {
    Board board;
    HasamiShogiAI ai1(Cell::UP);
    HasamiShogiAI ai2(Cell::DOWN);
    bool gameOver = false;
    int currentPlayer = 1;

    cout << "AI vs AI の試合を開始します。\n";

    // ゲームループ
    while (!gameOver) {
        if (currentPlayer == 1) {
            if (aiTurn(board, ai1, 1)) {
                // 勝敗判定
                int ai1Count = board.countPlayerPiece(ai1.my_piece);
                int ai2Count = board.countPlayerPiece(ai2.my_piece);

                if (ai2Count == 0) {
                    cout << "AI 1 の勝利！\n";
                    gameOver = true;
                } else if (ai1Count == 0) {
                    cout << "AI 2 の勝利！\n";
                    gameOver = true;
                }

                currentPlayer = 2;  // 次はAI 2のターン
            }
        } else {
            if (aiTurn(board, ai2, -1)) {
                // 勝敗判定
                int ai1Count = board.countPlayerPiece(ai1.my_piece);
                int ai2Count = board.countPlayerPiece(ai2.my_piece);

                if (ai2Count == 0) {
                    cout << "AI 1 の勝利！\n";
                    gameOver = true;
                } else if (ai1Count == 0) {
                    cout << "AI 2 の勝利！\n";
                    gameOver = true;
                }

                currentPlayer = 1;  // 次はAI 1のターン
            }
        }

        // ターンの間に少し待機（視認性向上のため）
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    cout << "ゲーム終了。\n";
    return 0;
}
