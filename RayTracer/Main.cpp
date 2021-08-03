#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<cmath>
#include<climits>
#include<string>
#include "vect.h"
#include "Ray.h"
#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Sphere.h"
#include "Object.h"
#include "Plane.h"
#include "Source.h"
#include <algorithm>
#include <time.h>
#include "Triangle.h"
using namespace std;

struct RGBtype {
	double r, g, b;
};

int winningObjectIndex(vector<double> object_intersections) {
	// return the index of the winning intersection
	// whatever obj is at smaller distance
	int index_of_minimum_value;

	// prevent unnessary calculations
	if (object_intersections.size() == 0) {
		// if there are no intersections	
		return -1;
	}
	else if (object_intersections.size() == 1) {
		if (object_intersections.at(0) > 0) {
			// if that intersection is greater than zero then its our index of minimum value
			return 0;
		}
		else {
			// otherwise the only intersection value is negative
			return -1;
		}
	}
	else {
		// otherwise there is more than one intersection
		// first find the mximum value

		double mx = 0;
		// = *max_element(object_intersections.begin(), object_intersections.end());

		for (int i = 0; i < (int)object_intersections.size(); i++) {
			if (mx < object_intersections.at(i)) {
				mx = object_intersections.at(i);
			}
		}

		// then starting from the maximum value find the minimum positive value
		if (mx > 0) {
			// we only want positive intersections
			for (int index = 0; index < (int)object_intersections.size(); index++) {
				if (object_intersections.at(index) > 0 && object_intersections.at(index) <= mx) {
					mx = object_intersections.at(index);
					index_of_minimum_value = index;
				}
			}
			return index_of_minimum_value;
		}
		else {
			// all the intersections were negative
			return -1;
		}
	}
}

void saveImage(const char *fileName, int width, int height, int dpi, RGBtype *data) {
	FILE* f;
	int area = 4 * width * height;
	int fileSize = 54 + area;

	double factor = 39.375;
	int m = static_cast<int>(factor);

	// DPI -> dots per inch = PPI
	// PPM -> Pixel Per Meter
	int ppm = dpi * width;
	
	// format of header
	// http://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm

	unsigned char bmpfileHeader[14] = { 'B','M', 0,0,0,0, 0,0,0,0, 0,0,0,0 };
	unsigned char bmpInfoHeader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0 };

	// identify the BMP and DIB BM
	// 4 bytes that stores size of file
	// 2 bytes Reserved; actual value depends on the application that creates the image
	// 2 bytes Reserved; actual value depends on the application that creates the image
	// 4 bytes starting address, of the byte where the bitmap image data (pixel array) can be found.
	bmpfileHeader[2] = (unsigned char)(fileSize);
	bmpfileHeader[3] = (unsigned char)(fileSize>>8);
	bmpfileHeader[4] = (unsigned char)(fileSize>>16);
	bmpfileHeader[5] = (unsigned char)(fileSize>>24);
	
	// Horizontal width of BMP in Pixel
	bmpInfoHeader[4] = (unsigned char)(width);
	bmpInfoHeader[5] = (unsigned char)(width>>8);
	bmpInfoHeader[6] = (unsigned char)(width>>16);
	bmpInfoHeader[7] = (unsigned char)(width>>24);

	// Vertical Height of BMP in Pixel
	bmpInfoHeader[8] = (unsigned char)(height);
	bmpInfoHeader[9] = (unsigned char)(height>>8);
	bmpInfoHeader[10] = (unsigned char)(height>>16);
	bmpInfoHeader[11] = (unsigned char)(height>>24);

	// ImageSize
	bmpInfoHeader[21] = (unsigned char)(area);
	bmpInfoHeader[22] = (unsigned char)(area >> 8);
	bmpInfoHeader[23] = (unsigned char)(area >> 16);
	bmpInfoHeader[24] = (unsigned char)(area >> 24);

	// horizontal resolution: Pixels/meter
	bmpInfoHeader[25] = (unsigned char)(ppm);
	bmpInfoHeader[26] = (unsigned char)(ppm >> 8);
	bmpInfoHeader[27] = (unsigned char)(ppm >> 16);
	bmpInfoHeader[28] = (unsigned char)(ppm >> 24);
	
	// Vertical resolution: Pixels/meter
	bmpInfoHeader[29] = (unsigned char)(ppm);
	bmpInfoHeader[30] = (unsigned char)(ppm >> 8);
	bmpInfoHeader[31] = (unsigned char)(ppm >> 16);
	bmpInfoHeader[32] = (unsigned char)(ppm >> 24);

	f = fopen(fileName, "wb");
	fwrite(bmpfileHeader, 1, 14, f);
	fwrite(bmpInfoHeader, 1, 40, f);

	for (int i = 0; i < height * width; ++i) {
		auto rgb = data[i];
		double red = (data[i].r) * 255;
		double green = (data[i].g) * 255;
		double blue = (data[i].b) * 255;

		unsigned char color[3] = { (unsigned char)floor(blue), (unsigned char)floor(green), (unsigned char)floor(red) };

		fwrite(color, 1, 3, f);
	}
	fclose(f);
}

Color getColorAt(vect intersection_Position, vect intersecting_Ray_Direction, vector<Object*> scene_objects, vector<Source*> light_Sources, double index_of_winning_object, double accuracy, double ambient_Light) {
	// Looping through Lighsources

	// Brithness at perticular point depends on 2 parameeter
	// 1. Distance
	// 2. quality of light can shine
	vect winning_Object_Normal = scene_objects[index_of_winning_object]->getNormalAt(intersection_Position);
	Color winning_Object_Color = scene_objects[index_of_winning_object]->getColor();

	// tile pattern
	if (winning_Object_Color.getColorSpecial() == 2) {
		int square = (int)intersection_Position.getVectX() + (int)intersection_Position.getVectZ();
		if (square % 2 == 0) {
			// black
			winning_Object_Color.setColorRed(0);
			winning_Object_Color.setColorGreen(0);
			winning_Object_Color.setColorBlue(0);
		}
		else{
			winning_Object_Color.setColorRed(1);
			winning_Object_Color.setColorGreen(1);
			winning_Object_Color.setColorBlue(1);
		}
	}

	Color final_Color = winning_Object_Color.colorScalar(ambient_Light);

	if (winning_Object_Color.getColorSpecial() > 0 && winning_Object_Color.getColorSpecial() <= 1) {
		// Find reflection direction
		double c1 = winning_Object_Normal.dot_Product(intersecting_Ray_Direction.nagative());
		vect temp = winning_Object_Normal.vect_Multiply_Scalar(c1).vect_Multiply_Scalar(2);
		vect reflection_direction = intersecting_Ray_Direction.vectAdd(temp);

		Ray reflection_Ray(intersection_Position, reflection_direction);

		vector<double> reflection_Intersections;

		for (int i = 0; i < scene_objects.size(); ++i) {
			reflection_Intersections.push_back(scene_objects[i]->findIntersection(reflection_Ray));
		}

		int index_of_winning_object_reflected_ray = winningObjectIndex(reflection_Intersections);

		if (index_of_winning_object_reflected_ray != -1) {
			// Miss
			if (reflection_Intersections[index_of_winning_object_reflected_ray] > accuracy) {
				vect reflection_Intersection_Position = intersection_Position.vectAdd(reflection_direction.vect_Multiply_Scalar(reflection_Intersections[index_of_winning_object_reflected_ray]));
				vect reflection_Intersection_Direction = reflection_direction;

				//vect intersection_Position, vect intersecting_Ray_Direction, vector<Object*> scene_objects,
				//vector<Source*> light_Sources, 
				//double index_of_winning_object, double accuracy, double ambient_Light
				Color at_Reflection_Intersection_Color = getColorAt(reflection_Intersection_Position, reflection_Intersection_Direction, 
					scene_objects, light_Sources, index_of_winning_object_reflected_ray, accuracy, ambient_Light);

				final_Color = final_Color.colorAdd(at_Reflection_Intersection_Color.colorScalar(winning_Object_Color.getColorSpecial()));
			}
		}
	}

	for (int light = 0; light < light_Sources.size(); ++light) {
		vect light_Direction = light_Sources[light]->getLightPosition().vectAdd(intersection_Position.nagative()).normalize();
		
		// p*q = pqcos(theta) agar normal vector he to p,q = 1 so cos(theta) = p*q
		float cosine = winning_Object_Normal.dot_Product(light_Direction);
		 
		if (cosine > 0) {
			bool shadow = false;

			// Distance to light source from intersection point
			vect distance_to_Light = light_Sources[light]->getLightPosition().vectAdd(intersection_Position.nagative()).normalize();
			float curr_light_Magnitude = distance_to_Light.magnitude();
			
			// in the direction from our intersection point to the light source
			// Abhi iss array me koi intersect hoga on it's way to light source
			// If yes then shadow hoy

			Ray shadow_Ray(intersection_Position, light_Sources[light]->getLightPosition().vectAdd(intersection_Position.nagative()).normalize());

			vector<double> shadow_Chk_Intersections;
			for (int i = 0; i < scene_objects.size(); ++i) {
				shadow_Chk_Intersections.push_back(scene_objects[i]->findIntersection(shadow_Ray));
			}

			for (int i = 0; i < shadow_Chk_Intersections.size(); ++i) {
				if (shadow_Chk_Intersections[i] > accuracy) {
					if (shadow_Chk_Intersections[i] <= curr_light_Magnitude) {
						shadow = true;
					}
					break;	
				}
			}
			if (shadow == false) {
				final_Color = final_Color.colorAdd(winning_Object_Color.colorMultiply(light_Sources[light]->getLightColor()).colorScalar(cosine));

				// shyniness
				if (winning_Object_Color.getColorSpecial() > 0 && winning_Object_Color.getColorSpecial() <= 1) {
					// special between 0 and 1 then syniness
					/*double dot1 = winning_Object_Normal.dot_Product(intersecting_Ray_Direction.nagative());
					vect scalar1 = winning_Object_Normal.vect_Multiply_Scalar(dot1);
					vect add1 = scalar1.vectAdd(intersecting_Ray_Direction);
					vect scalar2 = add1.vect_Multiply_Scalar(2);
					vect add2 = intersecting_Ray_Direction.nagative().vectAdd(scalar2);
					vect reflection_direction = add2.normalize();*/,

					/*c1 = -dot_product(N, V)
						Rl = V + (2 * N * (-n*v))*/

					double c1 = winning_Object_Normal.dot_Product(intersecting_Ray_Direction.nagative());
					vect temp = winning_Object_Normal.vect_Multiply_Scalar(c1).vect_Multiply_Scalar(2);
					vect reflection_direction = intersecting_Ray_Direction.vectAdd(temp);

					double specular = reflection_direction.dot_Product(light_Direction);
					if (specular > 0) {
						specular = pow(specular, 10);
						final_Color = final_Color.colorAdd(light_Sources[light]->getLightColor().colorScalar((specular) * (winning_Object_Color.getColorSpecial())));
					}
				}
			}
		}
	}

	return final_Color.make_bounded();
}

void makeCube(vect c1, vect c2, Color color, vector<Object*>& scene_Object) {
	double c1_X = c1.getVectX();
	double c1_Y = c1.getVectY();
	double c1_Z = c1.getVectZ();

	double c2_X = c2.getVectX();
	double c2_Y = c2.getVectY();
	double c2_Z = c2.getVectZ();

	vect A(c2_X, c1_Y, c1_Z);
	vect B(c2_X, c1_Y, c2_Z);
	vect C(c1_X, c1_Y, c2_Z);
	vect D(c2_X, c2_Y, c1_Z);
	vect E(c1_X, c2_Y, c1_Z);
	vect F(c1_X, c2_Y, c2_Z);

	// Left Side
	scene_Object.push_back(new Triangle(D, A, c1, color));
	scene_Object.push_back(new Triangle(c1, E, D, color));
	// scene_Object.push_back(new Triangle(D, E, c1, color));

	// Right Side
	scene_Object.push_back(new Triangle(F, C, B, color));
	scene_Object.push_back(new Triangle(B, c2, F, color));
	// scene_Object.push_back(new Triangle(F, c2, B, color));

	// Top Side
	scene_Object.push_back(new Triangle(D, E, F, color));
	scene_Object.push_back(new Triangle(F, c2, D, color));
	// scene_Object.push_back(new Triangle(D, c2, F, color));

	// Bottom Side
	scene_Object.push_back(new Triangle(c1, A, B, color));
	scene_Object.push_back(new Triangle(B, C, c1 , color));
	// scene_Object.push_back(new Triangle(c1, C, B , color));

	// Far side
	scene_Object.push_back(new Triangle(c2, B, A, color));
	scene_Object.push_back(new Triangle(A, D, c2, color));
	//scene_Object.push_back(new Triangle(c1, D, A, color));

	// Front Side
	scene_Object.push_back(new Triangle(E, c1, C, color));
	scene_Object.push_back(new Triangle(C, F, E, color));
	// scene_Object.push_back(new Triangle(E, F, C, color));
}

int main() {

	clock_t start, end;
	start = clock();

	cout << "Rendering.." << endl;
	int width = 640;
	int height = 480;
	int dpi = 72; 

	int n = width * height; // Total Number of pixel in image
	RGBtype* pixels = new RGBtype[n];

	vect _X(1, 0, 0);
	vect _Y(0, 1, 0);
	vect _Z(0, 0, 1);	
	vect _O(0, 0, 0);

	// defining direction of camera
	vect camPos(3, 1.5, -4); // arbitory setting of camera

	// same karu to badhi baju image sarki felay

	// 0,0,0 to render j na thay 
	// 5,5,5 -> small small thatu jay -> 0,20,20 sm -> 0,50,50 sm sm -> 0, 100,100 sm sm sm -> full bk
	// 50,50,0 -> full bk -> small as sm sm
	// 50, 0, 50 -> eczactly half back and half bk and center ma sphere
	// -5,-5,-5 -> bahar nu background j dekhay

	vect look_At(0, 0, 0);

	//displaced camera positions are taken in the plane perpendicular to the camera view direction.
	// Forward Vector
	vect diff_BTW(camPos.getVectX() - look_At.getVectX(), camPos.getVectY() - look_At.getVectY(), camPos.getVectZ() - look_At.getVectZ());
	vect camDir = diff_BTW.nagative().normalize();

	// Forward * y
	vect camRight = _Y.cross_Product(camDir).normalize();

	// Right * forword
	vect camDown = camRight.cross_Product(camDir);

	// camright, camdown used to define general coordinate system 
	// also used in method function which is in vector in order to compute our perspective	

	Camera scene_Cam(camPos, camDir, camRight, camDown);
	// We have Raytracer, models camera with OBJ programming

	Color white_light(1.0, 1.0, 1.0, 0);
	
	Color pretty_green(0.2,.1,.9, 0.3);
	Color gray(0.5, 0.5, 0.5, 0);
	Color black(0.0, 0.0, 0.0, 0);
	Color marron(0.5,.25,.25,0);
	Color tile_Color(1,1,1, 2);
	Color orange(.1,.9,1, 0);

	// setting of light_Position
	vect light_Position(-7, 10, -10);	
	
	// vector, color
	Light scene_Light(light_Position, white_light);  
	
	vector<Source*> light_Sources; 
	light_Sources.push_back(dynamic_cast<Source*>(&scene_Light));
	
	// Scene Objects
	Sphere scene_Sphere(_O, 1, pretty_green);
	
	// _Y normal deneka -> center, radius, color
	Plane scene_Plane(_Y, -1, tile_Color);
	vect new_Sphere_location(1.2, -.5, 0.5);
	Sphere scene_Sphere2(new_Sphere_location, 0.5, marron);
	Triangle scene_Triangle(vect(2, 0, 0), vect(0, 2, 0), vect(0, 0, 10), orange);

	// proportion between width : height
	double aspectratio = (double)width / (double)height;
	
	vector<Object*> scene_objects;	
	scene_objects.push_back(dynamic_cast<Object*>(&scene_Sphere));
	scene_objects.push_back(dynamic_cast<Object*>(&scene_Plane));
	scene_objects.push_back(dynamic_cast<Object*>(&scene_Sphere2));
	// scene_objects.push_back(dynamic_cast<Object*>(&scene_Triangle));

	makeCube(vect(2,0.6,.6), vect(2.5,1.2,1.2), orange, scene_objects);

	// slightly to right and slightly to left from our camera
	double xamt = 0, yamt = 0;	

	// we will ensure that intersection value must be out side of sphere
	double accuracy = .000001;
	// Light jo zyda padegi to image light hogi
	// light kam to image dark

	double ambient_Light = 0.2;

	const int depth = 1; // depth[1] -> if 2 send 4 new ray from a 1 pixel & depth[0] -> what's write now n so on
	int aa_Index = 1;
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			// Filling
			// Going Through Every Pixel in Image
			int foreach = j * width + i;

			// Anti Aliasing techniques 
			// avaraging color of our multiple pixel within another image place behind our first image plane
			// super sampling, area sampling, masking
			double aa_Red[depth * depth];
			double aa_Green[depth * depth];
			double aa_Blue[depth * depth];

			// i am passing multiple ray to pixels 
			for (int x = 0; x < depth; ++x) {
				for (int y = 0; y < depth; ++y) {

					aa_Index = y * depth + x;
					srand(time(0));
					if (depth == 1) {
						if (width > height) {
							// the image is wider than it is tall
							xamt = ((i + 0.5) / width) * aspectratio - (((width - height) / (double)height) / 2);
							yamt = ((height - j) + 0.5) / height;
						}
						else if (height > width) {
							// the imager is taller than it is wide
							xamt = (i + 0.5) / width;
							yamt = (((height - j) + 0.5) / height) / aspectratio - (((height - width) / (double)width) / 2);
						}
						else {
							// the image is square
							xamt = (i + 0.5) / width;
							yamt = ((height - j) + 0.5) / height;
						}
					}
					else {
						if (width > height) {
							// the image is wider than it is tall
							xamt = ((i + (double)x / ((double)depth - 1)) / width) * aspectratio - (((width - height) / (double)height) / 2);
							yamt = ((height - j) + (double)x / ((double)depth - 1)) / height;
						}
						else if (height > width) {
							// the imager is taller than it is wide
							xamt = ((i + (double)x / (double)depth - 1) / width);
							yamt = (((height - j) + (double)x / ((double)depth - 1)) / height) / aspectratio - (((height - width) / (double)width) / 2);
						}
						else {
							// the image is square
							xamt = (x + (double)x / ((double)depth - 1)) / width;
							yamt = ((height - y) + (double)x / ((double)depth - 1)) / height;
						}

					}

					// cout << xamt << " " << yamt << endl;

					vect cam_ray_origin = scene_Cam.getCameraPosition();
					// xamt and yamt is already on the maximize to we have decrese somewhat part so that we can see whole image
					vect cam_ray_direction = camDir.vectAdd(camRight.vect_Multiply_Scalar(xamt - 0.5).vectAdd(camDown.vect_Multiply_Scalar(yamt - .5))).normalize();
					// cout << cam_ray_origin.getVectX() << " " << cam_ray_direction.getVectX() << endl;
					Ray cam_ray(cam_ray_origin, cam_ray_direction);

					vector<double> intersections;

					// Ray which I have created is intersect with object which is in our scene
					for (int index = 0; index < (int)scene_objects.size(); index++) {
						// cout << scene_objects[index]->findIntersection(cam_ray) << " ";
						intersections.push_back(scene_objects.at(index)->findIntersection(cam_ray));
					}

					/*if (i == 0 && j == 0) {
						cout << intersections.size() << endl;
						copy(interections.begin(), intersections.end(), ostream_iterator<double>(cout, " "));
						cout << "\n";
					}*/

					/*for (int i = 0; i < intersections.size(); ++i) {
						cout << intersections[i] << endl;
					}*/

					// check which object is near to camera
					int index_of_winning_object = winningObjectIndex(intersections);

					// determine shadows at each point
					// find intersection go to light source
					// if we find any object btw intersection and light source than we have shadow

					// cout << index_of_winning_object;
					// Instead of color to object we color to intersecting point
					if (index_of_winning_object == -1) {
						// set the backgroung black
						aa_Red[aa_Index] = 0;
						aa_Green[aa_Index] = 0;
						aa_Blue[aa_Index] = 0;
					}
					else {
						if (intersections[index_of_winning_object] > accuracy) {

							// position, direction

							vect intersection_Position = cam_ray_origin.vectAdd(cam_ray_direction.vect_Multiply_Scalar(intersections[index_of_winning_object]));
							vect intersecting_Ray_Direction = cam_ray_direction; 
							// direction of intersecting ray is where ray is coming from -> so it is coming from camera

							//Reflection hoy to bija badha point ke jythi aave e b jovanu
							// Shadows, reflection, shyniness, spectral intensity
							Color intersection_Color = getColorAt(intersection_Position, intersecting_Ray_Direction, scene_objects, light_Sources, index_of_winning_object, accuracy, ambient_Light);
							aa_Red[aa_Index] = intersection_Color.getColorRed();
							aa_Green[aa_Index] = intersection_Color.getColorGreen();
							aa_Blue[aa_Index] = intersection_Color.getColorBlue();
						}
					}

					//if (index_of_winning_object == -1) {
					//	// Set Background black
					//	pixels[foreach].r = 0;
					//	pixels[foreach].g = 0;
					//	pixels[foreach].b = 0;
					//}
					//else {
					//	// Object randered
					//	Color object_Color = scene_objects[index_of_winning_object]->getColor();
					//	pixels[foreach].r = object_Color.getColorRed();
					//	pixels[foreach].g = object_Color.getColorGreen();
					//	pixels[foreach].b = object_Color.getColorBlue();
					//}

					/*if ((i > 200 && i < 440) && (j > 200 && j < 280)) {
						pixels[foreach].r = 12;
						pixels[foreach].g = 12;
						pixels[foreach].b = 100;
					}
					else {
						pixels[foreach].r = 0;
						pixels[foreach].g = 0;
						pixels[foreach].b = 0;
					}*/
				}
			}
			// average the pixel color
			double totalRed = 0;
			double totalGreen = 0;
			double totalBlue = 0;

			for (int iRed = 0; iRed < depth * depth; iRed++) {
				totalRed = totalRed + aa_Red[iRed];
			}
			for (int iGreen = 0; iGreen < depth * depth; iGreen++) {
				totalGreen = totalGreen + aa_Green[iGreen];
			}
			for (int iBlue = 0; iBlue < depth * depth; iBlue++) {
				totalBlue = totalBlue + aa_Blue[iBlue];
			}

			double avgRed = totalRed / (depth * depth);
			double avgGreen = totalGreen / (depth * depth);
			double avgBlue = totalBlue / (depth * depth);

			pixels[foreach].r = avgRed;
			pixels[foreach].g = avgGreen;
			pixels[foreach].b = avgBlue;
 		}
	}

	saveImage("Scene.bmp", width, height, dpi, pixels);

	delete pixels;

	end = clock();
	float excetime = ((float)end - (float)start);
	std::cout << excetime/1000 << " Seconds";

	return 0;
}