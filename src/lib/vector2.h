#ifndef _H_VECTOR2_
#define _H_VECTOR2_

class vector2 {
	
	float X, Y;
	friend class CGE;

public:

	vector2(float x, float y);
	void set_vector(float x, float y);
	void move_vector(float x, float y);
	float getX() {return X;};
	float getY() {return X;};
};

#endif