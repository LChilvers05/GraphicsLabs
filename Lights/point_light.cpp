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

Vector PointLight::get_direction() {
    return direction;
}

Colour PointLight::get_intensity() {
    return intensity;
}

bool PointLight::get_direction(Vertex &surface, Vector &dir) {
    dir = surface - position; dir.normalise();
    return true;
}

void PointLight::get_intensity(Vertex &surface, Colour &level) {
    level = intensity;
}
