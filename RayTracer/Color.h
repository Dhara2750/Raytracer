#ifndef COLOR_H
#define COLOR_H

#include "math.h"

class Color {
	double red, green, blue, special;
public:
	Color();
	Color(double, double, double, double);

	double getColorRed() { return red; }
	double getColorGreen() { return green; }
	double getColorBlue() { return blue; } 
	double getColorSpecial() { return special; }

	void setColorRed(double redVal) { red = redVal; }
	void setColorGreen(double greenVal) { green = greenVal; }
	void setColorBlue(double blueVal) { blue = blueVal; }
	void setColorSpecial(double specialVal) { special = specialVal; }

	double brightNess() {
		return (red + green + blue) / 3;
	}
	Color colorScalar(double scalar) {
		// Special color do like surface reflective, shineness
		return Color(red * scalar, green * scalar, blue * scalar, special);
	}

	Color colorAdd(Color color) {
		return Color(red + color.getColorRed(), green + color.getColorGreen(), blue + color.getColorBlue(), special + color.getColorSpecial());
	}

	Color colorMultiply(Color color) {
		return Color(red * color.getColorRed(), green * color.getColorGreen(), blue * color.getColorBlue(), special);
	}

	Color colorAverage(Color color) {
		return Color((red + color.getColorRed()) / 2, (green + color.getColorGreen()) / 2, (blue + color.getColorBlue()) / 2, special);
	}

	Color make_bounded() {
		double all_Light = red + green + blue;
		double excesslight = all_Light - 3;
		/*if (excesslight > 0) {
			red = red + excesslight * (red / all_Light);
			green = green + excesslight * (green / all_Light);
			blue = blue + excesslight * (blue / all_Light);
		}*/
		if (red > 1) { red = 1; }
		if (green > 1) { green = 1; }
		if (blue > 1) { blue = 1; }
		if (red < 0) { red = 0; }
		if (green < 0) { green = 0; }
		if (blue < 0) { blue = 0; }

		return Color(red, green, blue, special);
	}
};

Color::Color() {
	red = 0.5;
	green = 0.5;
	blue = 0.5;
}

Color::Color(double x, double y, double z, double s) {
	red = x;
	green = y;
	blue = z;
	special = s;
}

#endif // !COLOR_H