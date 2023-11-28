#include "vertex.h"
#include "vector.h"
#include "Lights/point_light.h"
#include "object.h"

class Photon {
public:

    float t; // distance to source light
    Colour intensity;
    Object* what;
    Vertex position;
    Vector normal;
    PhotonType type;
    PointLight* source;

    Photon(Colour intensity, Object* what, Vertex position, Vector normal, PointLight* source) {
        this->intensity = intensity;
        this->what = what;
        this->position = position;
        this->normal = normal;
        this->source = source;
        this->t = (source->get_position() - position).length();
    }

    void set_intensity(Colour intensity) {
        this->intensity = intensity;
    }

    void set_type(PhotonType type) {
        this->type = type;
    }

    void* operator new(size_t size);
    void operator delete(void* p);
};

enum PhotonType {
    Direct,
    Indirect,
    Shadow
};
