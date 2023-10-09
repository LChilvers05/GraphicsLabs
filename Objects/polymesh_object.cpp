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

    // read file into stream
    fstream file_stream;
    file_stream.open(file, ios::in);

    if (file_stream.is_open()) {
        //get line by line of file and split by spaces into vector
        string file_line;
        while (getline(file_stream, file_line, ' ')) {
            vector<string> split_line;
            split_line.push_back(file_line);
            process(split_line);
        }

    } else {
        std::cout << "Couldn't open file";
    }

    file_stream.close();
    next = 0;
}

void PolyMesh::process(vector<string> file_line) {
    if (file_line.size() < 1) { return; }

    //TODO: check for vertex ('v') and face ('f') lines
    if (file_line[0] == "v") {
        process_vertex(file_line);
    } else if (file_line[0] == "f") {
        process_face(file_line);
    } else { return; }
}

void PolyMesh::process_vertex(vector<string> raw_vertex) {
    if (raw_vertex.size() < 4) { return; }

    try {
        Vertex p (
            std::stof(raw_vertex[1]),
            std::stof(raw_vertex[2]),
            std::stof(raw_vertex[3])
        );

        vertex.push_back(p);

    } catch (const std::invalid_argument& e) {
        std::cout << "Could not convert vertex point to float";
    }
}

void PolyMesh:: process_face(vector<string> raw_face) {

}

//TODO: ignore for Lab 2
Hit* PolyMesh::intersection(Ray ray) {
    Hit* hits = 0;

    return hits;
}

void PolyMesh::apply_transform(Transform& trans) {

}