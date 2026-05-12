#ifndef pawn_h
#define pawn_h
#include <cmath>
#include "peice.h"

class Pawn:public Peice {
public:
	Pawn(bool w) :Peice(w) {}
	char getsymbol(){
		if (white) return 'P';

			return 'p';
	}

	bool isValidMove(int sr, int sc, int dr, int dc) {
        int dir = white ? -1 : 1;
        int startRow = white ? 6 : 1;

        if (dc == sc && dr == sr + dir){ return true; }

        if (dc == sc && sr == startRow && dr == sr + (2 * dir)){ return true; }

        if (abs(dc - sc) == 1 && dr == sr + dir){ return true; }

        return false;
    }
};

#endif pawn.h
