#ifndef AARECT_H
#define AARECT_H

#include "util.h"
#include "objeto.h"
#include "vec3.h"

class xyRect: public objeto {
  public:
    shared_ptr<material> mat;
    double x0, x1, y0, y1, k; //z = k

  public:
    xyRect() {}

    xyRect(double _x0, double _x1, double _y0, double _y1, double _k, shared_ptr<material> m) :
      x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mat(m) {};

    virtual bool hit(const ray&, double t0, double t1, hitRecord& rec) const;

    virtual bool boundingBox(double t0, double t1, aabb& outputBox) const {
      outputBox = aabb(point3(x0,y0, k-0.0001), point3(x1,y1, k+0.0001));
      return true;
    }
};

class xzRect: public objeto {
  public:
    shared_ptr<material> mat;
    double x0, x1, z0, z1, k; //y = k

  public:
    xzRect() {}

    xzRect(double _x0, double _x1, double _z0, double _z1, double _k, shared_ptr<material> m) :
      x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mat(m) {};

    virtual bool hit(const ray&, double t0, double t1, hitRecord& rec) const;

    virtual bool boundingBox(double t0, double t1, aabb& outputBox) const {
      outputBox = aabb(point3(x0, k-0.0001, z0), point3(x1, k+0.0001, z1));
      return true;
    }
};

class yzRect: public objeto {
  public:
    shared_ptr<material> mat;
    double y0, y1, z0, z1, k; //x = k

  public:
    yzRect() {}

    yzRect(double _y0, double _y1, double _z0, double _z1, double _k, shared_ptr<material> m) :
      y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mat(m) {};

    virtual bool hit(const ray&, double t0, double t1, hitRecord& rec) const;

    virtual bool boundingBox(double t0, double t1, aabb& outputBox) const {
      outputBox = aabb(point3(k-0.0001, y0, z0), point3(k+0.0001, y1, z1));
      return true;
    }
};

bool xyRect::hit(const ray& r, double t0, double t1, hitRecord& rec) const {
  auto t = (k-r.origem().z()) / r.direcao().z();
  if(t < t0 || t > t1)
    return false;
  auto x = r.origem().x() + t*r.direcao().x();
  auto y = r.origem().y() + t*r.direcao().y();
  if(x < x0 || x > x1 || y < y0 || y > y1)
    return false; //fora
  rec.u = (x-x0)/(x1-x0);
  rec.v = (y-y0)/(y1-y0);
  rec.t = t;

  auto normal_lado = vec3(0,0,1);
  rec.setFaceNormal(r, normal_lado);
  rec.ptrMat = mat;
  rec.p = r.at(t);
  return true;
}

bool xzRect::hit(const ray& r, double t0, double t1, hitRecord& rec) const {
  auto t = (k-r.origem().y()) / r.direcao().y();
  if(t < t0 || t > t1)
    return false;
  auto x = r.origem().x() + t*r.direcao().x();
  auto z = r.origem().z() + t*r.direcao().z();
  if(x < x0 || x > x1 || z < z0 || z > z1)
    return false; //fora
  rec.u = (x-x0)/(x1-x0);
  rec.v = (z-z0)/(z1-z0);
  rec.t = t;

  auto normal_lado = vec3(0,1,0);
  rec.setFaceNormal(r, normal_lado);
  rec.ptrMat = mat;
  rec.p = r.at(t);
  return true;
}


bool yzRect::hit(const ray& r, double t0, double t1, hitRecord& rec) const {
  auto t = (k-r.origem().x()) / r.direcao().x();
  if(t < t0 || t > t1)
    return false;
  auto y = r.origem().y() + t*r.direcao().y();
  auto z = r.origem().z() + t*r.direcao().z();
  if( y < y0 || y > y1 || z < z0 || z > z1)
    return false; //fora
  rec.u = (y-y0)/(y1-y0);
  rec.v = (z-z0)/(z1-z0);
  rec.t = t;

  auto normal_lado = vec3(1,0,0);
  rec.setFaceNormal(r, normal_lado);
  rec.ptrMat = mat;
  rec.p = r.at(t);
  return true;
};

#endif
