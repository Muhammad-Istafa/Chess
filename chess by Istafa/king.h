#ifndef king_h
#define king_h
#include "peice.h"

class King :public Peice {
public:
	King(bool w) :Peice(w) {}
	char getsymbol() {
		if (white) return 'K';

		return 'k';
	}
};

#endif king.h