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
	const float b_q = 2*((a * p_x * d_x) + b*(p_x*d_y + d_x*p_y) + c*(p_x*d_z + d_x*p_z) + (d * d_x) + (e * p_y * d_y) + f*(p_y*d_z + d_y*p_z) + (g * d_y) + (h * p_z * d_z) + (i * d_z));
	const float c_q = (a * p_x*p_x) + (2*b * p_x * p_y) + (2*c * p_x * p_z) + (2*d * p_x) + (e * p_y*p_y) + (2*f * p_y * p_z) + (2*g * p_y) + (h * p_z*p_z) + (2*i * p_z) + j;

	const float disc = (b_q*b_q) - (4 * a_q * c_q);

	// no intersection and ignore if tangent
	if (disc < 0 || a_q == 0) return 0;

	// quadratic get both values for t
	const float t0 = (-b_q - sqrtf(disc)) / (2*a_q);
	const float t1 = (-b_q + sqrtf(disc)) / (2*a_q);

	// create hits and check if visible to ray
	bool hit0_isVisible;
	Hit* hit0 = make_hit(ray, t0, hit0_isVisible);

	bool hit1_isVisible;
	Hit* hit1 = make_hit(ray, t1, hit1_isVisible);

	Hit* hits = 0;
	if (hit0_isVisible && hit1_isVisible) {

		if (hit0->t < hit1->t) {
			hit0->entering = true;
			hit0->next = hit1;
			hit1->entering = false;
			hits = hit0;

		} else {
			hit1->entering = true;
			hit1->next = hit0;
			hit0->entering = false;
			hits = hit1;
		}

	} else if (hit0_isVisible) {
		delete hit1;
		hit0->entering = false;
		hits = hit0;

	} else if (hit1_isVisible) {
		delete hit0;
		hit1->entering = false;
		hits = hit1;

	} else {
		delete hit0;
		delete hit1;
	} 

	return hits;
}

Hit* Quadratic::make_hit(const Ray ray, const float t, bool& isVisible) {
	const float p_x = ray.position.x, p_y = ray.position.y, p_z = ray.position.z;
	const float d_x = ray.direction.x, d_y = ray.direction.y, d_z = ray.direction.z;

	Hit* hit = new Hit();
	hit->what = this;
	hit->next = 0;
	hit->position = Vertex(
		p_x + (t*d_x), 
		p_y + (t*d_y), 
		p_z + (t*d_z)
	);
	Vector direction = hit->position - ray.position;
	hit->t = direction.length();
	direction.normalise();
	// check if hit is before the ray starting position
	isVisible = (direction.dot(ray.direction) > 0);

	// normal
	hit->normal = Vector(
		2*(a*hit->position.x + b*hit->position.y + c*hit->position.z + d),
		2*(b*hit->position.x + e*hit->position.y + f*hit->position.z + g),
		2*(c*hit->position.x + f*hit->position.y + h*hit->position.z + i)
	);
	hit->normal.normalise();
	if (hit->normal.dot(ray.direction) > 0) hit->normal.negate();

	return hit;
}

void Quadratic::apply_transform(Transform& trans) {}