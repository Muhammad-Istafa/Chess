#ifndef bishop_h
#define bishop_h
#include<cmath>
#include "peice.h"

class Bishop :public Peice {
public:
	Bishop (bool w) :Peice(w) {}
	char getsymbol() {
		if (white) return 'B';

		return 'b';
	}

	bool isValidMove(int sr, int sc, int dr, int dc) {
    	if (abs(sr - dr) == abs(sc - dc)) return true;
	    return false;
	}

	bool isSlider() {
    	return true;
	}
};

#endif bishop.h
