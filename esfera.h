#ifndef ESFERA_H
#define ESFERA_H

#include "objeto.h"
#include "vec3.h"

class esfera: public objeto {
  public:
    point3 centro;
    double raio;
    shared_ptr<material> ptrMat;

  public:
    esfera() {}
    esfera(point3 cen, double r, shared_ptr<material> material) : centro(cen), raio(r), ptrMat(material){};

    virtual bool hit(const ray& r, double tmin, double tmax, hitRecord& rec) const;

    virtual bool boundingBox(double t0, double t1, aabb& outputBox) const;
};

void getCordsEsfera(const vec3& p, double& u, double& v) {
  auto phi = atan2(p.z(), p.x());
  auto theta = asin(p.y());
  u = 1 - (phi + pi) / (2*pi);
  v = (theta + pi/2) / pi;
}

bool esfera::hit(const ray& r, double t_min, double t_max, hitRecord& rec) const {
  vec3 oc = r.origem() - centro;
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
     rec.normal = (rec.p - centro) / raio;
     getCordsEsfera((rec.p - centro)/raio, rec.u, rec.v);
     vec3 normal_lado = (rec.p - centro) / raio;
     rec.setFaceNormal(r, normal_lado);
     rec.ptrMat = ptrMat;
     return true;
    }
    raiz_aux = (-b + raiz)/a;
    if(raiz_aux < t_max && raiz_aux > t_min){
     rec.t = raiz_aux;
     rec.p = r.at(rec.t);
     rec.normal = (rec.p - centro) / raio;
     getCordsEsfera((rec.p - centro)/raio, rec.u, rec.v);
     vec3 normal_lado = (rec.p - centro) / raio;
     rec.setFaceNormal(r, normal_lado);
     rec.ptrMat = ptrMat;
     return true;
    }
  }
  return false;
};

bool esfera::boundingBox(double t0, double t1, aabb& outputBox) const {
  outputBox = aabb(
      centro - vec3(raio, raio, raio),
      centro + vec3(raio, raio, raio));
  return true;
}


#endif
