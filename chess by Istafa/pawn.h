#ifndef pawn_h
#define pawn_h
#include "peice.h"

class Pawn:public Peice {
public:
	Pawn(bool w) :Peice(w) {}
	char getsymbol(){
		if (white) return 'P';

			return 'p';
	}
};

#endif pawn.h