#ifndef SOURCE_H
#define SOURCE_H

class Source {
public:
	Source();
	virtual vect getLightPosition() { return vect(0, 0, 0); }
	virtual Color getLightColor() { return Color(1, 1, 1, 0); }
};

Source::Source() {}

#endif // !SOURCE_H
