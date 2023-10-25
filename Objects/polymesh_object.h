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
#include "plane_object.h"

typedef int TriangleIndex[3];

class PolyMesh:public Object{
public:

	int triangle_count;
	vector<Vertex> vertex;
	vector<Vector> vertex_normals;
	vector<vector<int> > triangle;
	vector<vector<int> > triangle_normals;

	bool smooth_render;

	Hit *intersection(Ray ray);
	void apply_transform(Transform& trans);

    PolyMesh(char *file, bool smooth);
	~PolyMesh(){}

private:	

	void read_file(char* file, bool smooth);
	vector<string> split(string str, char ch);
	void process(vector<string> line_components);
	void process_vertex(vector<string> raw_vertex);
	void process_vertex_normal(vector<string> raw_vertex_normal);
	void process_face(vector<string> raw_face);
	Vector get_face_normal(const vector<int>& tri, const vector<Vertex>& vertex);
	void interpolate_vertex_normals(
		const vector<int>& tri, 
		const vector<Vertex>& vertex, 
		const vector<int>& tri_n, 
		const vector<Vector>& vertex_normal, 
		Hit*& hit);
	bool ray_does_intersect(
    	const Vector& normal,
    	const Vertex& a_v,
    	const Ray ray,
    	Vertex& pos
	);
	void populate_vertex_normals();
    void register_hit(
		Hit*& hit, 
		const Ray ray, 
		const Vertex hit_pos,
    	const vector<int> tri_n, 
		const vector<int> tri,
        const Vector normal
	);
};
