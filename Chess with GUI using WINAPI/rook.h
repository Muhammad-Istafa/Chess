#ifndef rook_h
#define rook_h
#include "peice.h"

class Rook :public Peice {
public:
	Rook(bool w) :Peice(w) {}
	char getsymbol() {
		if (white) return 'R';

		return 'r';
	}

	bool isValidMove(int sr, int sc, int dr, int dc){
		if(sr == dr || sc == dc) return true;
		return false;
	}

	bool isSlider() {
    	return true;
	}
};

#endif rook.h
