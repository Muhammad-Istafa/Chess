#ifndef king_h
#define king_h
#include<cmath>
#include "peice.h"

class King :public Peice {
public:
	King(bool w) :Peice(w) {}
	char getsymbol() {
		if (white) return 'K';

		return 'k';
	}

	bool isValidMove(int sr, int sc, int dr, int dc){
		if((abs(sr-dr)<=1) && (abs(sc-dc)<=1) && !(sr==dr && sc==dc)) return true;
		return false;
	}
};

#endif king.h
