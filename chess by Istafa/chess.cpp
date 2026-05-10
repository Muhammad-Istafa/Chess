#include "board.h"
#include <iostream>
using namespace std;

int main() {
    Board chessboard;
    bool whiteTurn = true;
    int sr, sc, dr, dc;

    cout << "  === CHESS GAME ===" << endl;

    while (true) {
        chessboard.display();
        cout << (whiteTurn ? "WHITE's turn" : "BLACK's turn") << endl;
        cout << "Enter move (sr sc dr dc): ";
        cin >> sr >> sc >> dr >> dc;

        bool moved = chessboard.movePiece(sr, sc, dr, dc);
        if (moved) {
            whiteTurn = !whiteTurn;
        }
    }

    return 0;
}
