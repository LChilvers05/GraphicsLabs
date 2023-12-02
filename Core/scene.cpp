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

#include "scene.h"
#include <vector>
#include <algorithm>

Scene::Scene() {
    is_photon_mapping = false;
    object_list = 0;
    light_list = 0;

    Photon min(Vertex(-999, -999, -999));
  	Photon max(Vertex(999, 999, 999));
    typedef KD::Core<3, Photon> CORE;
  	kd_tree = new KD::Tree<CORE>(min, max);
}

// Pass 1: Constructing the Photon Maps
void Scene::construct_photon_map(int photon_count) {
    is_photon_mapping = true;

    Light* light = light_list;
    while (light != (Light*) 0) {
        // cast to PointLight
        PointLight* point_light = dynamic_cast<PointLight*>(light);
        if (point_light == nullptr) continue;
        // shoot photons into scene
        for (int i = 0; i < photon_count; i++) {
            Ray ray = create_light_ray(
                point_light->get_position(), 
                point_light->get_direction()
            );
            light_trace(ray, point_light, 0);
        }

        light = light->next;
    }
}

void Scene::light_trace(Ray ray, PointLight* source, int depth) {
    if (depth > 3) return;

    Hit* hit_p = this->trace(ray);
    if (hit_p == 0) return;

    Hit hit = *hit_p;

    Photon photon = Photon(
        source->get_intensity(), 
        hit.what, 
        hit.position, 
        hit.normal, 
        source
    );

    // decide reflected or absorbed
    float p_d, p_s;
    hit.what->material->get_diffuse_specular_probs(p_d, p_s, hit, ray);
    // Russian Roulette
    float r = random_float(0.f, 1.f);
    if (r < p_d) {
        // absorb
        photon.set_type((depth == 0) ? 1 : 2);
        photon.set_intensity(photon.intensity * (1.f/p_d)); //TODO: check
        kd_tree->insert(photon); 

        // do diffuse reflection
        Ray rray = create_light_ray(photon.get_position(), photon.normal);
        light_trace(rray, source, depth+1);

    } else if (r < p_d + p_s) {
        // do specular reflection
        Ray rray;
        rray.direction = ray.direction - ((2.0 * photon.normal.dot(ray.direction)) * photon.normal);
        rray.position = photon.get_position() + (0.0001f * rray.direction);
        light_trace(rray, source, depth+1);

    } else {
        // absorb
        photon.set_type(1);
        photon.set_intensity(photon.intensity * (1.f/p_d)); //TODO: check
        kd_tree->insert(photon);
    }

    // shadow photons
    // point far along ray and reverse ray
    Vertex sray_pos = photon.get_position() + (300.f * ray.direction);
    Vector sray_dir = ray.direction.negated();
    Hit* shadow_hit_p = 0;
    while(true) {
        // check every hit until reach original photon
        Ray sray = Ray(sray_pos, sray_dir);
        shadow_hit_p = this->trace(sray);

        if (shadow_hit_p == 0 ||
            shadow_hit_p->position.isEqual(photon.get_position())) break;
        Hit shadow_hit = *shadow_hit_p;

        Colour shadow_intensity = shadow_hit.what->material->ambient;
        Photon shadow_photon = Photon(
            shadow_intensity, 
            shadow_hit.what, 
            shadow_hit.position, 
            shadow_hit.normal, 
            source
        );
        shadow_photon.set_type(3);
        kd_tree->insert(shadow_photon);

        sray_pos = shadow_photon.get_position() + (0.0001f * sray_dir);
        
        delete shadow_hit_p;
    }

    delete hit_p;
    delete shadow_hit_p;
}

Ray Scene::create_light_ray(Vertex pos, Vector dir) {

    Vector ldir = Vector(
        random_float(-100.f, 100.f),
        random_float(-100.f, 100.f),
        random_float(-100.f, 100.f)
    );
    ldir.normalise();
    if (ldir.dot(dir) < 0) ldir.negate();

    return Ray(pos + (0.0001f * ldir), ldir);
}

Hit *Scene::trace(Ray ray) {
    Hit *best_hit = 0;

    Object *objects = this->object_list;

    while (objects != 0) {
        Hit *hit = this->select_first(objects->intersection(ray));

        if (hit != 0) {
            if (best_hit == 0) {
                best_hit = hit;

            } else if (hit->t < best_hit->t) {
                delete best_hit;
                best_hit = hit;
            } else {
                delete hit;
            }
        }

        objects = objects->next;
    }

    return best_hit;
}

bool Scene::shadowtrace(Ray ray, float limit) {
    Object *objects = this->object_list;

    while (objects != 0) {
        Hit *hit = this->select_first(objects->intersection(ray));

        if (hit != 0) {
            if ((hit->t > 0.00000001f) && (hit->t < limit)) {
                delete hit;
                return true;
            }
            delete hit;
        }

        objects = objects->next;
    }

    return false;
}

void Scene::raytrace(Ray ray, int recurse, Colour &colour, float &depth) {
    Object *objects = object_list;
    Light *lights = light_list;

    // a default colour if we hit nothing.
    colour.r = 0.0f;
    colour.g = 0.0f;
    colour.b = 0.0f;
    colour.a = 0.0f;
    depth = 0.0f;

    // first step, find the closest primitive
    Hit *best_hit = this->trace(ray);

    // if we found a primitive then compute the colour we should see
    if (best_hit != 0) {
        depth = best_hit->t;
        // this will be the global components
        // such as ambient or reflect/refract
        colour = colour + best_hit->what->material->compute_once(ray, *best_hit, recurse);

        bool is_shadow_tracing = true;
        bool is_accurate_calc = true;

        // Pass 2: Rendering with photon map
        if (is_photon_mapping) {
            try {
                vector<Photon> photons = kd_tree->within(Photon(best_hit->position), 1.0);

                is_accurate_calc = false;

                Colour sum_intensity;
                for (int i = 0; i < photons.size(); i++) { 
                    Photon p = photons[i];
                    sum_intensity = sum_intensity + p.intensity; 
                }
                Colour average_intensity = sum_intensity * (1.f / photons.size());
                colour = colour + average_intensity;
            } catch (...) {
                int x = 1;
            }

            // is_accurate_calc = false;

            // bool has_direct = false;
            // bool has_indirect = false;
            // bool has_shadow = false;

            // Colour sum_intensity;
            // for (int i = 0; i < photons.size(); i++) {
            //     Photon p = photons[i];
            //     sum_intensity = sum_intensity + p.intensity;

            //     if (photons[i].type == 1) {
            //         has_direct = true;
            //     } else if (photons[i].type == 2) {
            //         has_indirect = true;
            //     } else if (photons[i].type == 3) {
            //         has_shadow = true;
            //     }
            // }

            // if (has_direct && !has_indirect && !has_shadow) {
            //     // sample is only direct photons
            //     is_shadow_tracing = false;
            //     is_accurate_calc = true;
            // } else if (has_direct && !has_indirect && has_shadow) {
            //     // sample is direct and shadow photons
            //     is_shadow_tracing = true;
            //     is_accurate_calc = true;
            // } else if (has_direct && has_indirect && has_shadow) {
            //     // sample is direct, indirect and shadow photons
            //     is_accurate_calc = false;

            //     Colour average_intensity = sum_intensity * (1.f / photons.size());
            //     colour = colour + average_intensity;
            // }
        }
        //!!!

        if (is_accurate_calc) {
            // next, compute the light contribution for each light in the scene.
            Light *light = light_list;
            while (light != (Light *)0) {
                Vector ldir;
                Vector viewer = -ray.direction;
                viewer.normalise();

                bool lit = light->get_direction(best_hit->position, ldir);
                ldir.normalise();

                // so shadow ray doesn't start behind object
                Vertex sray_pos = best_hit->position - (0.0001f * ldir);
                Ray sray = Ray(sray_pos, -ldir);

                float limit = 999.f;
                PointLight *point_light = dynamic_cast<PointLight *>(light);
                if (point_light != nullptr) {
                    // decide shadow based on position of light
                    limit = (point_light->get_position() - sray_pos).length();
                }

                // light is facing wrong way.
                if (ldir.dot(best_hit->normal) > 0) {
                    lit = false;
                }

                if (is_shadow_tracing) {


                    bool inShadow = shadowtrace(sray, limit);
                    if (lit == true && inShadow) {
                        lit = false;
                    }
                }

                if (lit) {
                    Colour intensity;
                    light->get_intensity(best_hit->position, intensity);

                    colour = colour +
                            intensity *
                            best_hit->what->material->compute_per_light(
                                viewer, 
                                *best_hit,
                                ldir
                            );
                }

                light = light->next;
            }
        }

        delete best_hit;
    } else {
        colour.r = 0.0f;
        colour.g = 0.0f;
        colour.b = 0.0f;
        colour.a = 1.0f;
    }
}

Hit *Scene::select_first(Hit *list) {
    Hit *result = 0;

    while (list != 0) {
        if (list->t >= 0.0f) {
            result = list;
            list = list->next;
            break;
        }

        Hit *temp = list;
        list = list->next;
        delete temp;
    }

    while (list != 0) {
        Hit *temp = list;
        list = list->next;
        delete temp;
    }

    return result;
}

void Scene::add_object(Object *obj) {
    obj->next = this->object_list;
    this->object_list = obj;
}

void Scene::add_light(Light *light) {
    light->next = this->light_list;
    this->light_list = light;
}
