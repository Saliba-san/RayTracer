#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
  public:
    double tm;
    point3 orig;
    vec3 dir;

  public:
    ray() {}
    ray(const point3& origin, const vec3& direction, const double t = 0.0) : orig(origin), dir(direction), tm(t) {}

    point3 origem() const {return orig;}
    vec3 direcao() const {return dir;}
    double tempo() const {return tm;}

    point3 at(double t) const {
      return t*dir + orig;
    }
};
#endif

