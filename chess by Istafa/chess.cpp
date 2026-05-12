#include "board.h"
#include <iostream>
#include <cstdlib>
#include <windows.h>
using namespace std;

int main() {
    Board chessboard;
    bool whiteTurn = true;
    int sr, sc, dr, dc;

    cout << "  === CHESS GAME ===" << endl;

    while (true) {
        system("cls");
        chessboard.display();
        cout << (whiteTurn ? "WHITE's turn" : "BLACK's turn") << endl;
        cout << "Enter move (sr sc dr dc): ";
        cin >> sr >> sc >> dr >> dc;

        bool moved = chessboard.movePiece(sr, sc, dr, dc);
        if (moved) {
            if (chessboard.kingCaptured) {
                chessboard.display();
                cout << (whiteTurn ? "WHITE wins!" : "BLACK wins!") << endl;
                break;
            }
            whiteTurn = !whiteTurn;
        } else {
            cout << "  Invalid move! Try again." << endl;
            Sleep(1000);
        }
    }

    return 0;
}
