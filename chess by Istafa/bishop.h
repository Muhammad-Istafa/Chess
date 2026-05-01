#ifndef bishop_h
#define bishop_h
#include "peice.h"

class Bishop :public Peice {
public:
	Bishop (bool w) :Peice(w) {}
	char getsymbol() {
		if (white) return 'B';

		return 'b';
	}
};

#endif bishop.h