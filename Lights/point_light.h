#pragma once
#include "light.h"

class PointLight : public Light {
public:

	PointLight();
	PointLight(Vertex pos, Vector dir, Colour col);

    Vertex get_position();
	bool get_direction(Vertex &surface, Vector &dir);
	void get_intensity(Vertex &surface, Colour &intensity);

private:
    Vertex position;
	Vector direction;
	Colour intensity;
};
