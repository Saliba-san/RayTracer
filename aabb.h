#ifndef AABB_H
#define AABB_H

#include "ray.h"
#include "util.h"
#include <type_traits>

class aabb {
  public:
   point3 maxB;
   point3 minB; 

  public:
   aabb() {}
   aabb(const point3& a, const point3 b) : maxB(b), minB(a) {}
   
   point3 min() const {return minB;}
   point3 max() const {return maxB;}

   bool hit(const ray& r, double tmin, double tmax) const {
     for (int a = 0; a < 3; a++) {
       auto invD = 1.0 / r.direcao()[a];
       auto t0 = (min()[a] - r.origem()[a]) * invD;
       auto t1 = (max()[a] - r.origem()[a]) * invD;
       if(invD < 0.0)
         std::swap(t0, t1);
       tmin = t0 > tmin? t0 : tmin;
       tmax = t1 < tmax? t1 : tmax;
       if (tmax <= tmin)
         return false;
     }
     return true;
   }

   aabb surroundingBox(aabb box0, aabb box1){
     point3 small(
         fmin(box0.min().x(), box1.min().x()),
         fmin(box0.min().y(), box1.min().y()),
         fmin(box0.min().z(), box1.min().z())
         );
     point3 big(
         fmax(box0.max().x(), box1.max().x()),
         fmax(box0.max().y(), box1.max().y()),
         fmax(box0.max().z(), box1.max().z())
         );
     return aabb(small, big);
   }
};
#endif
