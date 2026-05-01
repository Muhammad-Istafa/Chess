#ifndef peice_h
#define peice_h

class Peice {
protected:
	bool white;
public:
	Peice(bool w) {
		white = w;
	}
	bool iswhite (){
		return white;
	}
	virtual char getsymbol() = 0;
	~Peice() {};
};

#endif 
