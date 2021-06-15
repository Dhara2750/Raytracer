#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <iostream>

#include "Color.h"
#include "Object.h"
#include "vect.h"

class Triangle : public Object {
	vect a, b, c;
	vect normal;
	double distance;
	Color color;
public:
	Triangle();
	Triangle(vect, vect, vect, Color);

	// Normal vector is vector which is perpendicular to our surface
	vect getTriangleNormal() { 
		vect ca(c.getVectX() - a.getVectX(), c.getVectY() - a.getVectY(), c.getVectZ() - a.getVectZ());
		vect ba(b.getVectX() - a.getVectX(), b.getVectY() - a.getVectY(), b.getVectZ() - a.getVectZ());
		return ca.cross_Product(ba).normalize();
	}
	double getTriangleDistance() { 
		// Distance of plane from origin
		normal = getTriangleNormal();
		distance = normal.dot_Product(a);
		return distance; 
	}
	Color getColor() { return color; }

	vect getNormalAt(vect point) {
		return getTriangleNormal();
	}

	double findIntersection(Ray ray) {
		vect ray_direction = ray.getrayDirection();
		vect ray_origin = ray.getRayOrigin();

		normal = getTriangleNormal();
		distance = getTriangleDistance();
		//std::cout << "Ray Direction : " << ray_direction.getVectX() << " " << ray_direction.getVectY() << " " << ray_direction.getVectZ() << "\n";
		double temp = ray_direction.dot_Product(normal);
		if (temp == 0) {
			// ray is parllel to our plane
			return -1;
		}
		else {
			double dis = normal.dot_Product(ray.getRayOrigin().vectAdd(normal.vect_Multiply_Scalar(distance).nagative()));
			// cam to intersection of point in that plane (-1) * b / temp;	

			double disbtw = (-1) * dis / temp;

			// Triangle per intersection
			double intersectionX = ray_origin.getVectX() + ray_direction.vect_Multiply_Scalar(disbtw).getVectX();
			double intersectionY = ray_origin.getVectY() + ray_direction.vect_Multiply_Scalar(disbtw).getVectY();
			double intersectionZ = ray_origin.getVectZ() + ray_direction.vect_Multiply_Scalar(disbtw).getVectZ();

			vect p(intersectionX, intersectionY, intersectionZ);

			// a, b, c and p;

			vect ba(b.getVectX() - a.getVectX(), b.getVectY() - a.getVectY(), b.getVectZ() - a.getVectZ());
			vect cb(c.getVectX() - b.getVectX(), c.getVectY() - b.getVectY(), c.getVectZ() - b.getVectZ());
			vect ac(a.getVectX() - c.getVectX(), a.getVectY() - c.getVectY(), a.getVectZ() - c.getVectZ());
			ba = ba.nagative();
			cb = cb.nagative();
			ac = ac.nagative();
			vect pa(p.getVectX() - a.getVectX(), p.getVectY() - a.getVectY(), p.getVectZ() - a.getVectZ());
			vect pb(p.getVectX() - b.getVectX(), p.getVectY() - b.getVectY(), p.getVectZ() - b.getVectZ());
			vect pc(p.getVectX() - c.getVectX(), p.getVectY() - c.getVectY(), p.getVectZ() - c.getVectZ());

			double cond1 = (ba.cross_Product(pa)).dot_Product(normal);
			double cond2 = (cb.cross_Product(pb)).dot_Product(normal);
			double cond3 = (ac.cross_Product(pc)).dot_Product(normal);
			
			if (cond1 >= 0 && cond2 >= 0 && cond3 >= 0) {
				// Inside 
				return (-1) * dis / temp;
			}
			else {
				// Miss the triangle
				return -1;
			}

		}
	}
};

Triangle::Triangle() {
	a = vect(1, 0, 0);
	b = vect(0, 1, 0);
	c = vect(0, 0, 1);
	color = Color(0.5, 0.5, 0.5, 0);
}

Triangle::Triangle(vect pointA, vect pointB, vect pointC, Color col) {
	a = pointA;
	b = pointB;
	c = pointC;
	color = col;
}

#endif // !TRIANGLE_H