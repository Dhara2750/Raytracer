#ifndef VECT_H
#define VECT_H

#include "math.h"

class vect {
	double x, y, z;
public:
	vect();
	vect(double, double, double);

	double getVectX() { return x; }
	double getVectY() { return y; }
	double getVectZ() { return z; }

	// Normalize
	double magnitude() {
		return sqrt((x * x) + (y * y) + (z * z));
	}
	vect normalize() {
		double magnitude = sqrt((x * x) + (y * y) + (z * z));
		return vect(x/magnitude, y/magnitude, z/magnitude);
	}
	vect nagative() {
		return vect(-x, -y, -z);
	}

	//what amount of one vector goes in the direction of another
	double dot_Product(vect v) {
		return x * v.getVectX() + y * v.getVectY() + z * v.getVectZ();
	}
	vect cross_Product(vect v) {
		// [x,-y, z] in sec place it's minus so term are reverted
		return vect(y * v.getVectZ() - z * v.getVectY(), z * v.getVectX() - x * v.getVectZ(), x * v.getVectY() - y * v.getVectX());
	}
	vect vectAdd(vect v) {
		return vect(x + v.getVectX(), y + v.getVectY(), z + v.getVectZ());
	}
	vect vect_Multiply_Scalar(double scalar) {
		return vect(x * scalar, y * scalar, z * scalar);
	}
};

vect::vect() {
	x = 0;
	y = 0; 
	z = 0;
}

vect::vect(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

#endif // !VECT_H