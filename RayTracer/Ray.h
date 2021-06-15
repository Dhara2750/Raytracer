#ifndef RAY_H
#define RAY_H

#include "math.h"
#include "vect.h"

class Ray {
	// origin, direction
	vect origin, direction;
public:
	Ray();
	Ray(vect, vect);

	vect getRayOrigin() { return origin; }
	vect getrayDirection() { return direction; }
};

Ray::Ray() {
	origin = vect(0, 0, 0);
	direction = vect(1, 0, 0);
}

Ray::Ray(vect o, vect d) {
	origin = o;
	direction = d;
}

#endif // !RAY_H