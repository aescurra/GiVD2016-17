#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "Material.h"

class Lambertian : public Material
{

public:
    //Ka, Kd, Ks
    Lambertian(const vec3& Ka,const vec3& color, const vec3& Ks, float shininess);
    virtual ~Lambertian();
    virtual bool scatter(const Ray& r_in, const HitInfo& rec, vec3& color, Ray& scattered) const;

};

#endif // LAMBERTIAN_H
