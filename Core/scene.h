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

#pragma once

#include "colour.h"
#include "ray.h"
#include "object.h"
#include "light.h"
#include "point_light.h"
#include "hit.h"
#include "environment.h"
#include "vertex.h"
#include "../Tree/tree.h"
#include "photon.h"

#include <iostream>
#include <random>


// Scene is a class that is used to build a scene database of objects
// and lights and then trace a ray through it.

class Scene: public Environment {
public:

  bool is_photon_mapping;
  Vertex scene_min;
  Vertex scene_max;
  
  // to store photons in
  typedef KD::Core<3, Photon> CORE;
  KD::Tree<CORE>* kd_tree;

  Object *object_list;
  Light *light_list;

  Scene();

  // Emit photons from light sources and construct photo map (pass 1)
  void construct_photon_map(int photon_count);

  Ray create_light_ray(Vertex pos, Vector dir);

  // Filter the list of returned hits to the closest +ve
  Hit* select_first(Hit* list);

  // Trace a ray through the scene and find the closest if any object
  // intersection in front of the ray.
  Hit *trace(Ray ray);
	
  // Trace a ray through the scene and return its colour. This function
  // is the one that should recurse down the reflection/refraction tree within a material
  void raytrace(Ray ray, int recurse, Colour& colour, float& depth);

  // raytrace a shadow ray.
  bool shadowtrace(Ray, float limit);

  // raytrace a light ray for photon mapping.
  void light_trace(Ray ray, Colour intensity, int depth);

  void add_object(Object *obj);
  void add_light(Light *light);

  float random_float(float min, float max) {
      std::random_device rand;
      std::mt19937 range(rand());
      std::uniform_real_distribution<float> distribution(min, max);
      return distribution(range);
  }
};
