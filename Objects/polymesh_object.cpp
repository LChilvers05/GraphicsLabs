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
    triangle_count = 0;
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
            stof(raw_vertex[3])
        );

        vertex.push_back(v);

    } catch (const std::invalid_argument& e) {
        printf("Could not convert vertex point to float");
    }
}

void PolyMesh:: process_face(vector<string> raw_face) {
    if (raw_face.size() < 4) { return; }
    //make face from f line
    triangle.push_back(vector<int>());
    for (int i = 1; i < raw_face.size(); i++) {
        vector<string> split_f = split(raw_face[i], '/');
        try {
            //obj v index start from 1 not 0
            triangle[triangle_count].push_back(stoi(split_f[0])-1);
        } catch (const std::invalid_argument& e) {
            printf("Could not convert face point to int");
        }
    }
    triangle_count++;
}

void PolyMesh::apply_transform(Transform& trans) {
    // for (int i = 0; i < vertex.size(); i++) {
    //     trans.apply(vertex[i]);
    // }
}

//TODO: ignore for Lab 2
Hit* PolyMesh::intersection(Ray ray) {
    Hit* hits = 0;

    return hits;
}
