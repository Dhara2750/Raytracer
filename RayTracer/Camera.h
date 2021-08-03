#ifndef CAMERA_H
#define CAMERA_H

#include "math.h"
#include "vect.h"

class Camera {
	// origin, direction
	vect camPos, camDir, camRight, camDown;
public:
	Camera();
	Camera(vect, vect, vect, vect);

	vect getCameraPosition() { return camPos; }
	vect getCameraDirection() { return camDir; }
	vect getCameraRight() { return camRight; }
	vect getCameraDown() { return camDown; }
};

Camera::Camera() {
	camPos = vect(0, 0, 0);
	camDir = vect(0, 0, 1);
	camRight = vect(0, 0, 0);
	camDown = vect(0, 0, 0);
}

Camera::Camera(vect camPos, vect camDir, vect camRight, vect camDown) {
	this->camPos = camPos;
	this->camDir = camDir;
	this->camRight = camRight;
	this->camDown = camDown;
}

#endif // !CAMERA_H