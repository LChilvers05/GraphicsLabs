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

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vertex.h>

#include "polymesh_object.h"

using namespace std;


PolyMesh::PolyMesh(char* file, bool smooth) {
    read_file(file, smooth);
    next = 0;
}

void PolyMesh::read_file(char* file, bool smooth) {
    // read file into stream
    fstream file_stream;
    file_stream.open(file, ios::in);

    if (file_stream.is_open()) {
        //get line by line of file
        string line;
        while (getline(file_stream, line)) {
            process(split(line, ' '));
        }

        triangle_count = triangle.size();

    } else {
        printf("Couldn't open file");
    }

    file_stream.close();
}

vector<string> PolyMesh::split(string str, char ch) {
    //clean
    if (str.back() == '\r') {
        str.erase(str.size()-1);
    }

    stringstream sstream(str);
    string elem;
    vector<string> split_str;
    //split line by char
    while (getline(sstream, elem, ch)) {
        split_str.push_back(elem);
    }

    return split_str;
}

void PolyMesh::process(vector<string> line) {
    if (line.size() < 1) { return; }
    //check for vertex and face lines
    if (!line[0].compare("v")) {
        process_vertex(line);
    } else if (!line[0].compare("vn")) {
        process_vertex_normal(line);
    } else if (!line[0].compare("vt")) {
        return;
    } else if (!line[0].compare("f")) {
        process_face(line);
    } else { return; }
}

void PolyMesh::process_vertex(vector<string> raw_vertex) {
    if (raw_vertex.size() < 4) { return; }
    //make vertex from v line
    try {
        Vertex v (
            stof(raw_vertex[1]),
            stof(raw_vertex[2]),
            stof(raw_vertex[3]) + 0.0001
        );

        vertex.push_back(v);

    } catch (const std::invalid_argument& e) {
        printf("Could not convert vertex point to float");
    }
}

void PolyMesh::process_vertex_normal(vector<string> raw_vertex_normal) {
    if (raw_vertex_normal.size() < 4) { return; }
    //make vertex normal from vn line
    try {
        Vector vn (
            stof(raw_vertex_normal[1]),
            stof(raw_vertex_normal[2]),
            stof(raw_vertex_normal[3])
        );
        vn.normalise();
        vertex_normals.push_back(vn);

    } catch (const std::invalid_argument& e) {
        printf("Could not convert vertex normal to float");
    }
}

void PolyMesh::process_face(vector<string> raw_face) {
    if (raw_face.size() < 4) { return; }
    //make face from f line
    try {
        //obj v index start from 1 not 0
        int a = stoi(split(raw_face[1], '/')[0]) - 1;
        //split a polygon into triangles
        for (int i = 2; i < raw_face.size() - 1; i++) {

            int b = stoi(split(raw_face[i], '/')[0]) - 1;
            int c = stoi(split(raw_face[i+1], '/')[0]) - 1;

            vector<int> tri;
            tri.push_back(a); tri.push_back(b); tri.push_back(c);
            triangle.push_back(tri);
            //get face normal with triangle vertices
            face_normals.push_back(get_face_normal(tri, vertex));
        }
    } catch (const std::invalid_argument& e) {
        printf("Could not convert face point to int");
    }
}

Vector PolyMesh::get_face_normal(const vector<int>& tri, const vector<Vertex>& vertex) {
    //three vertices of triangle
    Vertex a = vertex[tri[0]]; Vertex b = vertex[tri[1]]; Vertex c = vertex[tri[2]];
    //two direction vectors (edges of triangle)
    Vector ab = a.operator-(b); Vector ac = a.operator-(c);
    //normal is cross product of two edges
    Vector normal = Vector(); ab.cross(ac, normal);

    return normal;
}

void PolyMesh::apply_transform(Transform& trans) {
    for (int i = 0; i < vertex.size(); i++) {
        trans.apply(vertex[i]);
    }
}

//TODO: ignore for Lab 2
Hit* PolyMesh::intersection(Ray ray) {
    Hit* hits = 0;

    return hits;
}
