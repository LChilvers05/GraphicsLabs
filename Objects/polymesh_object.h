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

// The PolyMesh Object reads and intersects with triangle meshes

#pragma once

#include "object.h"
#include "vertex.h"

typedef int TriangleIndex[3];

class PolyMesh:public Object{
public:

	int triangle_count;
	vector<Vertex> vertex;
	vector<vector<int> > triangle;

	Hit *intersection(Ray ray);
	void apply_transform(Transform& trans);

    PolyMesh(char *file, bool smooth);
	~PolyMesh(){}

private:

	void process(vector<string> file_line);
	void process_vertex(vector<string> raw_vertex);
	void process_face(vector<string> raw_face);
};
