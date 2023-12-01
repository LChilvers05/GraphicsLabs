#include "object.h"
#include "point_light.h"
#include "vector.h"
#include "vertex.h"

class Photon {
   public:

    float position[3];  // for KD tree
    float t;            // distance to source light
    Colour intensity;
    Object* what;
    Vector normal;
    int type; //1 = direct, 2 = indirect, 3 = shadow
    PointLight* source;

    Photon() {}
    
    Photon(Vertex position) {
        this->position[0] = position.x;
        this->position[1] = position.y;
        this->position[2] = position.z;
    }

    Photon(Colour intensity, Object* what, Vertex position, Vector normal,
           PointLight* source) {
        this->intensity = intensity;
        this->what = what;
        this->normal = normal;
        this->source = source;
        this->t = (source->get_position() - position).length();

        this->position[0] = position.x;
        this->position[1] = position.y;
        this->position[2] = position.z;
    }

    Vertex get_position() {
        return Vertex(position[0], position[1], position[2]);
    }

    void set_intensity(Colour intensity) { this->intensity = intensity; }

    void set_type(int type) { this->type = type; }

    float operator[](int i) const { return position[i]; }

    bool operator==(const Photon& p) const {
        return position[0] == p[0] && position[1] == p[1] &&
               position[2] == p[2];
    }

    friend std::ostream& operator<<(std::ostream& s, const Photon& p) {
        return s << '(' << p[0] << ", " << p[1] << ", " << p[2] << ')';
    }

    void* operator new(size_t size);
    void operator delete(void* p);
};
