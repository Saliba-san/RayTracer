#ifndef BOX_H
#define BOX_H

#include "aabb.h"
#include "vec3.h"
#include "listaObjetos.h"
#include "objeto.h"
#include "aarect.h"

class box : public objeto {
  public:
    point3 boxMin;
    point3 boxMax;
    listaObjetos lados;

  public:
    box() {}
    box(const point3& p0, const point3& p1, shared_ptr<material> ptr);

    virtual bool hit (const ray& r, double t0, double t1, hitRecord& rec) const;

    virtual bool boundingBox(double t0, double t1, aabb& outputBox) const {
      outputBox = aabb(boxMin, boxMax);
      return true;
    }
};

box::box(const point3& p0, const point3& p1, shared_ptr<material> ptr) {
  boxMin = p0;
  boxMax = p1;

  lados.add(make_shared<xyRect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
  lados.add(make_shared<flipFace>(
        make_shared<xyRect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr)));

  lados.add(make_shared<xzRect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
  lados.add(make_shared<flipFace>(
        make_shared<xzRect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr)));

  lados.add(make_shared<yzRect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
  lados.add(make_shared<flipFace>(
        make_shared<yzRect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr)));
}

bool box::hit(const ray& r, double t0, double t1, hitRecord& rec) const {
  return lados.hit(r, t0, t1, rec);
}

#endif
