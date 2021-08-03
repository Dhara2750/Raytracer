#ifndef PLANE_H
#define PLANE_H

#include <iostream>

#include "Color.h"
#include "Object.h"
#include "vect.h"
// Normal vector is vector which is perpendicular to our surface -> which is normal
class Plane : public Object {
	vect normal;
	double distance;
	Color color;
public:
	Plane();
	Plane(vect, double, Color);

	vect getPlaneNormal() { return normal; }
	double getPlaneDistance() { return distance; }
	Color getColor() { return color; }

	vect getNormalAt(vect point) {
		return normal;
	}
	
	// Intersection between rays origin and plane
	// Or
	// Intersection between ray origin to the point of intersection
	double findIntersection(Ray ray) {
		vect ray_direction = ray.getrayDirection();
		//std::cout << "Ray Direction : " << ray_direction.getVectX() << " " << ray_direction.getVectY() << " " << ray_direction.getVectZ() << "\n";
		double temp = ray_direction.dot_Product(normal);
		if (temp == 0) {
			// ray is parllel to our plane
			return -1;
		}
		else {
			double first = normal.dot_Product(ray.getRayOrigin().vectAdd(normal.vect_Multiply_Scalar(distance).nagative()));
			double b = first;
			return (-1) * b / temp;
		}
	}
};

Plane::Plane() {
	// y -> left, right
	// z -> depth of our scene
	normal = vect(1, 0, 0);
	distance = 0;
	color = Color(0.5, 0.5, 0.5, 0);
}

Plane::Plane(vect normal, double distance, Color color) {
	this->normal = normal;
	this->distance = distance;
	this->color = color;
}

#endif // !PLANE_H