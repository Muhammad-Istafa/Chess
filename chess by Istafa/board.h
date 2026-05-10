#ifndef board_h
#define board_h
#include <iostream>
#include "peice.h"
#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"
class Board {
    Peice* BOARD[8][8];
public:
    Board();
    void setupBoard();
    void display();
    bool movePiece(int sr, int sc, int dr, int dc);
    bool kingCaptured;
};
#endif
