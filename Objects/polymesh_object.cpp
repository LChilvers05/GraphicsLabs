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

#include "polymesh_object.h"

using namespace std;


PolyMesh::PolyMesh(char* file, bool smooth) {

    // read file into stream
    fstream file_stream;
    file_stream.open(file, ios::in);

    if (file_stream.is_open()) {
        //get line by line of file
        string file_line;
        while (getline(file_stream, file_line)) {

            //TODO: process the file line, check for v and f
            //put into triangles and vertex
        }

    } else {
        std::cout << "Couldn't open file";
    }

    file_stream.close();
    next = 0;
}

//TODO: ignore for Lab 2
Hit* PolyMesh::intersection(Ray ray) {
    Hit* hits = 0;

    return hits;
}

void PolyMesh::apply_transform(Transform& trans) {

}