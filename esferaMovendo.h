#ifndef ESFERAMOVENDO_H
#define ESFERAMOVENDO_H

#include "material.h"
#include "objeto.h"

class esferaMovendo : public objeto {
  public:
    point3 centro0, centro1;
    double tempo0, tempo1;
    double raio;
    shared_ptr<material> ptrMat;

  public:
    esferaMovendo() {}
    esferaMovendo(
        point3 cen0, point3 cen1, double t0, double t1, double r, shared_ptr<material> mat) :
      centro0(cen0), centro1(cen1), tempo0(t0), tempo1(t1), raio(r), ptrMat(mat) {};

    virtual bool hit (const ray& r, double tmin, double tmax, hitRecord& rec) const;

    virtual bool boundingBox(double t0, double t1, aabb& outputBox) const;

    point3 centro(double tempo) const;
};

point3 esferaMovendo::centro(double tempo) const{
  return centro0 + ((tempo - tempo0) / (tempo1 - tempo0))*(centro1 - centro0);
};

bool esferaMovendo::hit(const ray& r, double t_min, double t_max, hitRecord& rec) const {
  vec3 oc = r.origem() - centro(r.tempo());
  auto a = r.direcao().lengthSquared();
  auto b = dot(oc, r.direcao());
  auto c = oc.lengthSquared() - raio*raio;
  auto discriminante = b*b - a*c;

  if(discriminante > 0) {
    auto raiz = sqrt(discriminante);
    auto raiz_aux = (-b - raiz)/a;
    if(raiz_aux < t_max && raiz_aux > t_min){
     rec.t = raiz_aux;
     rec.p = r.at(rec.t);
     vec3 normal_lado = (rec.p - centro(r.tempo())) / raio;
     rec.setFaceNormal(r, normal_lado);
     rec.ptrMat = ptrMat;
     return true;
    }
    raiz_aux = (-b + raiz)/a;
    if(raiz_aux < t_max && raiz_aux > t_min){
     rec.t = raiz_aux;
     rec.p = r.at(rec.t);
     vec3 normal_lado = (rec.p - centro(r.tempo())) / raio;
     rec.setFaceNormal(r, normal_lado);
     rec.ptrMat = ptrMat;
     return true;
    }
  }
  return false;
};

bool esferaMovendo::boundingBox(double t0, double t1, aabb& outputBox) const {
  aabb box0(
      centro(t0) - vec3(raio,raio,raio),
      centro(t0) + vec3(raio,raio,raio)
      );
  aabb box1(
      centro(t1) - vec3(raio,raio,raio),
      centro(t0) + vec3(raio,raio,raio)
      );
  outputBox = box0.surroundingBox(box0,box1);
  return true;
};

#endif
