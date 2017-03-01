#ifndef PLANE_H
#define PLANE_H


#include "Object.h"

class Plane: public Object
{
private:
    vec3 normal;
    vec3 pPuntual;
    float d;
public:
    Plane(vec3 normal,vec3 pPuntual, Material *m);
    bool hit(const Ray& r, float t_min, float t_max, HitInfo& rec) const;
    ~Plane (){}
};

#endif // PLANE_H
