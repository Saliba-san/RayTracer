#ifndef OBJETO_H
#define OBJETO_H

#include "ray.h"
#include "util.h"
#include "aabb.h"
class material;

struct hitRecord{
  point3 p;
  vec3 normal;
  double t;
  double u,v;
  bool frente;
  shared_ptr<material> ptrMat;

  inline void setFaceNormal(const ray& r, const vec3& normal_lado) {
    frente  = dot(r.direcao(), normal_lado) < 0;
    normal = frente ? normal_lado : -normal_lado;
  };
};

class objeto{
  public:
    virtual bool hit(const ray& r, double t_min, double t_max, hitRecord& rec) const = 0;

    virtual bool boundingBox(double t0, double t1, aabb& outputBox) const = 0;
};

class flipFace : public objeto {
  public:
    shared_ptr<objeto> ptr;

  public:
    flipFace(shared_ptr<objeto> p) : ptr(p) {}

    virtual bool hit(const ray& r, double t_min, double t_max, hitRecord& rec) const {
      if(!ptr->hit(r,t_min,t_max,rec))
        return false;
      rec.frente = !rec.frente;
      return true;
    }

    virtual bool boundingBox(double t0, double t1, aabb& outputBox) const{
      return ptr->boundingBox(t0, t1, outputBox);
    }
};

class translate: public objeto {
  public:
    shared_ptr<objeto> ptr;
    vec3 offset;

  public:
    translate(shared_ptr<objeto> p, const vec3& displacement)
      : ptr(p), offset(displacement) {}

    virtual bool hit(const ray& r, double t_min, double t_max, hitRecord& rec) const;
    virtual bool boundingBox(double t0, double t1, aabb& outputBox) const;
};

bool translate::hit (const ray& r, double t_min, double t_max, hitRecord& rec) const {
  ray movedRay(r.origem() - offset, r.direcao(), r.tempo());
  if (!ptr -> hit(movedRay, t_min, t_max, rec))
    return false;

  rec.p += offset;
  rec.setFaceNormal(movedRay, rec.normal);

  return true;
}

bool translate::boundingBox(double t0, double t1, aabb& outputBox) const {
  if(!ptr -> boundingBox(t0, t1, outputBox))
    return false;

  outputBox = aabb(
      outputBox.min() + offset,
      outputBox.max() + offset);

  return true;
};

class rotateY : public objeto {
  public:
    shared_ptr<objeto> ptr;
    double sinTheta;
    double cosTheta;
    bool hasbox;
    aabb bbox;

  public: 
    rotateY ( shared_ptr<objeto> p, double angle);

    virtual bool hit(const ray& r, double t_min, double t_max, hitRecord& rec) const;
    virtual bool boundingBox(double t0, double t1, aabb& outputBox) const {
      outputBox = bbox;
      return hasbox;
    }
};

rotateY::rotateY(shared_ptr<objeto> p, double angle) : ptr(p) {
  auto radians = grauRad(angle);
  sinTheta = sin(radians);
  cosTheta = cos(radians);
  hasbox = ptr-> boundingBox(0, 1, bbox);

  point3 min ( infinity, infinity, infinity);
  point3 max ( -infinity, -infinity, -infinity);

  for ( int i = 0; i <2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
        auto x = i*bbox.max().x() + (1-i)*bbox.min().x();
        auto y = j*bbox.max().x() + (1-i)*bbox.min().x();
        auto z = k*bbox.max().x() + (1-i)*bbox.min().x();

        auto newx = cosTheta*x + sinTheta*z;
        auto newz = -sinTheta*x + cosTheta*z;

        vec3 tester(newx, y, newz);

        for(int c = 0; c < 3; c++) {
          min[c] = fmin(min[c], tester[c]);
          max[c] = fmax(max[c], tester[c]);
        }
      }
    }
  }
  bbox = aabb(min, max);
};

bool rotateY::hit(const ray& r, double t_min, double t_max, hitRecord& rec) const{
  auto origem = r.origem();
  auto direcao = r.direcao();

  origem[0] = cosTheta*r.origem()[0] - sinTheta*r.origem()[2];
  origem[2] = sinTheta*r.origem()[0] + cosTheta*r.origem()[2];

  direcao[0] = cosTheta*r.direcao()[0] - sinTheta*r.direcao()[2];
  direcao[2] = sinTheta*r.direcao()[0] + cosTheta*r.direcao()[2];

  ray rotatedRay(origem, direcao, r.tempo());

  if(!ptr->hit(rotatedRay, t_min, t_max, rec))
    return false;

  auto p = rec.p;
  auto normal = rec.normal;

  p[0] = cosTheta*rec.p[0] + sinTheta*rec.p[2];
  p[2] = -sinTheta*rec.p[0] + cosTheta*rec.p[2];

  normal[0] = cosTheta*rec.normal[0] + sinTheta*rec.normal[2];
  normal[2] = -sinTheta*rec.normal[0] + cosTheta*rec.normal[2];

  rec.p = p;
  rec.setFaceNormal(rotatedRay, normal);

  return true;
};

#endif
