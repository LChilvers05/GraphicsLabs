#include "object.h"
#include "point_light.h"
#include "vector.h"
#include "vertex.h"

struct Photon {
   public:

    float data[3];  // for KD tree
    float t;            // distance to source light
    Colour intensity;
    Object* what;
    Vector normal;
    int type; //1 = direct, 2 = indirect, 3 = shadow
    PointLight* source;

    Photon() {
        this->data[0] = 0.0;
        this->data[1] = 0.0;
        this->data[2] = 0.0;
    }
    
    Photon(Vertex position) {
        this->data[0] = position.x;
        this->data[1] = position.y;
        this->data[2] = position.z;

        this->type = -1;
    }

    Photon(Colour intensity, Object* what, Vertex position, Vector normal,
           PointLight* source) {
        this->intensity = intensity;
        this->what = what;
        this->normal = normal;
        this->source = source;
        this->t = (source->get_position() - position).length();

        this->data[0] = position.x;
        this->data[1] = position.y;
        this->data[2] = position.z;

        this->type = -1;
    }

    Vertex get_position() {
        return Vertex(data[0], data[1], data[2]);
    }

    void set_intensity(Colour intensity) { this->intensity = intensity; }

    void set_type(int type) { this->type = type; }

    float operator[](int i) const { return data[i]; }

    bool operator==(const Photon& p) const {
        return data[0] == p[0] && data[1] == p[1] &&
               data[2] == p[2];
    }

    friend std::ostream& operator<<(std::ostream& s, const Photon& p) {
        return s << '(' << p[0] << ", " << p[1] << ", " << p[2] << ')';
    }
};
