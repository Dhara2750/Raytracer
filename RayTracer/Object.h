#ifndef OBJECT_H
#define OBJECT_H

#include "Color.h"
#include "vect.h"
#include "Ray.h"

class Object {
public:
	Object();
	// Object ne sphere banvu hoy to color apvano
	virtual Color getColor() { return Color(0, 0, 0, 0); }
	virtual vect getNormalAt(vect intersection_position) = 0;
	virtual double findIntersection(Ray ray) = 0;
};

Object::Object() { }

#endif // !OBJECT_H
