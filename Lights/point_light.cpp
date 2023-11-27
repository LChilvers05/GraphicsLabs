#include "point_light.h"

PointLight::PointLight() { Light(); }

PointLight::PointLight(Vertex pos, Vector dir, Colour col) {
	Light();
    position = pos;
	direction = dir; direction.normalise();
	intensity = col;
}

Vertex PointLight::get_position() {
    return position;
}

bool PointLight::get_direction(Vertex &surface, Vector &dir) {
	dir = direction;
    return true;
}

void PointLight::get_intensity(Vertex &surface, Colour &level) {
    level = intensity;
}
