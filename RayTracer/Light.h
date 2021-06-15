#ifndef LIGHT_H
#define LIGHT_H

#include "Color.h"
#include "vect.h"
#include "Source.h"

class Light : public Source{
	vect position;
	Color color;
public:
	Light();
	Light(vect, Color);

	vect getLightPosition() { return position; }
	Color getLightColor() { return color; }
};

Light::Light() {
	position = vect(0, 0, 0);
	color = Color(1, 1, 1, 0);
}

Light::Light(vect p, Color c) {
	position = p;
	color = c;
}

#endif // !LIGHT_H