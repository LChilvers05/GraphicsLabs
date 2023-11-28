#pragma once
#include "light.h"

class PointLight : public Light {
public:

	PointLight();
	PointLight(Vertex pos, Vector dir, Colour col);

    Vertex get_position();
	Vector get_direction();
	Colour get_intensity();
	bool get_direction(Vertex &surface, Vector &dir);
	void get_intensity(Vertex &surface, Colour &intensity);

private:
    Vertex position;
	Vector direction;
	Colour intensity;
};
