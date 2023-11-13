/****************************************************************************
 *
 * krt - Ken's Raytracer - Coursework Edition. (C) Copyright 1993-2022.
 *
 * I've put a lot of time and effort into this code. For the last decade
 * it's been used to introduce hundreds of students at multiple universities
 * to raytracing. It forms the basis of your coursework but you are free
 * to continue using/developing forever more. However, I ask that you don't
 * share the code or your derivitive versions publicly. In order to continue
 * to be used for coursework and in particular assessment it's important that
 * versions containing solutions are not searchable on the web or easy to
 * download.
 *
 * If you want to show off your programming ability, instead of releasing
 * the code, consider generating an incredible image and explaining how you
 * produced it.
 */

#include "quadratic_object.h"

#include <math.h>

using namespace std;

Quadratic::Quadratic(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j) {
	next = 0;
	// ax2 + 2bxy + 2cxz + 2dx + ey2 + 2fyz + 2gy + hz2 + 2iz + j = 0
	this->a = a, this->b = b, this->c = c, this->d = d, this->e = e; 
	this->f = f, this->g = g, this->h = h, this->i = i, this->j = j;
}

Hit* Quadratic::intersection(Ray ray) { 
	const float p_x = ray.position.x, p_y = ray.position.y, p_z = ray.position.z;
	const float d_x = ray.direction.x, d_y = ray.direction.y, d_z = ray.direction.z;

	const float a_q = (a * d_x*d_x) + (2*b * d_x * d_y) + (2*c * d_x * d_z) + (e * d_y*d_y) + (2*f * d_y * d_z) + (g * d_z*d_z);
	const float b_q = 2*((a * p_x * d_x) + b*(p_x*d_y + d_x*p_y) + c*(p_x*d_z + d_x*d_z) + (d * d_x) + (e * p_y * d_y) + f*(p_y*d_z + d_y*p_z) + (g * d_y) + (h * p_z * d_z) + (i * d_z));
	const float c_q = (a * p_x*p_x) + (2*b * p_x * p_y) + (2*c * p_x * p_z) + (2*d * p_x) + (e * p_y*p_y) + (2*f * p_y * p_z) + (2*g * p_y) + (h * p_z*p_z) + (2*i * p_z) + j;

	const float disc = (b_q*b_q) - (4 * a_q * c_q);

	// no intersection and ignore if tangent
	if (disc < 0 || a_q == 0) return 0;

	const float t0 = (-b_q - sqrtf(disc)) / (2*a_q);
	const float t1 = (-b_q + sqrtf(disc)) / (2*a_q);

	//TODO: think about how we will do entering/exiting for refraction
	// logic if ts are infront or behind start of ray

	Vertex hit_pos0 = Vertex(
		p_x + (t0*d_x),
		p_y + (t0*d_y),
		p_z + (t0*d_z)
	);

	Vertex hit_pos1 = Vertex(
		p_x + (t1*d_x),
		p_y + (t1*d_y),
		p_z + (t1*d_z)
	);
	
	return 0; 
}

void Quadratic::apply_transform(Transform& trans) {}