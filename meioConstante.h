#ifndef MEIOCONSTANTE_H
#define MEIOCONSTANTE_H

#include "objeto.h"
#include "vec3.h"
#include "aabb.h"
#include "texture.h"
#include "material.h"

class meioConstante : public objeto {
  public:
    shared_ptr<objeto> boundry;
    shared_ptr<material> fase;
    double negInvDens;

  public:
    meioConstante(shared_ptr<objeto> b, double d, shared_ptr<texture> a):
      boundry(b), negInvDens(-1/d) {
        fase = make_shared<isotropic>(a);
      }

    virtual bool hit(const ray& r, double t_min, double t_max, hitRecord& rec) const;

    virtual bool boundingBox(double t0, double t1, aabb& outputBox) const {
      return boundry->boundingBox(t0, t1, outputBox);
    }
};

bool meioConstante::hit(const ray& r, double t_min, double t_max, hitRecord& rec) const{
  hitRecord rec1,  rec2;

  if (!boundry->hit(r, -infinity, infinity, rec1))
    return false;

  if (!boundry->hit(r, rec1.t+0.0001, infinity, rec2))
    return false;

  if(rec.t < t_min) rec1.t = t_min;
  if(rec.t > t_max) rec2.t = t_max;

  if (rec1.t >= rec2.t)
    return false;

  if(rec1.t < 0) rec1.t = 0;

  const auto rayLength = r.direcao().length();
  const auto distInside = (rec2.t - rec1.t) * rayLength;
  const auto hitDist = negInvDens * log(randDouble());

  if(hitDist > distInside)
    return false;

  rec.t = rec1.t + hitDist/rayLength;
  rec.p = r.at(rec.t);

  rec.normal = vec3(1,0,0);
  rec.frente = true;
  rec.ptrMat = fase;
  return true;
}



  

#endif
