/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

// Phong is a child class of Material and implement the simple Phong
// surface illumination model.

#pragma once

#include "material.h"

class Phong : public Material {
public:

	Colour ambient;	
	float k_d, k_s; // diffuse and specular coefficients

	Phong(Colour ambient) {
		this->ambient = ambient;
		this->k_d = 0.5f;
		this->k_s = 0.4f;
	}

	Phong(Colour ambient, float k_d, float k_s) {
		this->ambient = ambient;
		this->k_d = k_d;
		this->k_s = k_s;
	}

	Colour compute_once(Ray& viewer, Hit& hit, int recurse);
	Colour compute_per_light(Vector& viewer, Hit& hit, Vector& ldir);
	void get_diffuse_specular_probs(float& p_d, float& p_s, Hit& hit, Ray ray);
	float get_diffuse(Hit& hit, const Vector& ldir);
	float get_specular(const Vector& viewer, Hit& hit, Vector& ldir);
};
