#ifndef SPHERE_H
#define SPHERE_H

#include "Color.h"
#include "Object.h"
#include "vect.h"

class Sphere : public Object {
	vect center;
	double radius;
	Color color;
public:
	Sphere();
	Sphere(vect, double, Color);

	vect getSpherecenter() { return center; }
	double getSphereRadius() { return radius; }
	Color getColor() { return color; }

	vect getNormalAt(vect point) {
		// normal always points away from the center of a sphere
		vect normal_Vect = point.vectAdd(center.nagative()).normalize();
		return normal_Vect;
	}

	// When ray comes to spehre
	double findIntersection(Ray ray) {
		vect ray_origin = ray.getRayOrigin();
		double ray_origin_x = ray_origin.getVectX();
		double ray_origin_y = ray_origin.getVectY();
		double ray_origin_z = ray_origin.getVectZ();

		vect ray_direction = ray.getrayDirection();
		double ray_direction_x = ray_direction.getVectX();
		double ray_direction_y = ray_direction.getVectY();
		double ray_direction_z = ray_direction.getVectZ();

		vect sphere_center = center;
		double sphere_center_x = sphere_center.getVectX();
		double sphere_center_y = sphere_center.getVectY();
		double sphere_center_z = sphere_center.getVectZ();

		double a = 1; // normalized
		double b = (2 * (ray_origin_x - sphere_center_x) * ray_direction_x) + (2 * (ray_origin_y - sphere_center_y) * ray_direction_y) + (2 * (ray_origin_z - sphere_center_z) * ray_direction_z);
		double c = pow(ray_origin_x - sphere_center_x, 2) + pow(ray_origin_y - sphere_center_y, 2) + pow(ray_origin_z - sphere_center_z, 2) - (radius * radius);

		double discriminant = b * b - 4 * a * c;

		if (discriminant > 0) {
			/// the ray intersects the sphere

			// the first root
			double root_1 = ((-1 * b - sqrt(discriminant)) / 2) - 0.000001;

			if (root_1 > 0) {
				// the first root is the smallest positive root
				return root_1;
			}
			else {
				// the second root is the smallest positive root
				double root_2 = ((sqrt(discriminant) - b) / 2) - 0.000001;
				return root_2;
			}
		}
		else {
			// missed the sphere
			return -1;
		}
	}
};

Sphere::Sphere() {
	center = vect(0, 0, 0);
	radius = 1.0;
	color = Color(0.5, 0.5, 0.5, 0);
}

Sphere::Sphere(vect cen, double rad, Color c) {
	center = cen;
	radius = rad;
	color = c;
}

#endif // !SPHERE_H