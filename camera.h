#ifndef CAMERA_H
#define CAMERA_H

#include "util.h"
#include "vec3.h"
#include "ray.h"

class camera{
  private:
    point3 origem;
    point3 cantoInfEsq;
    vec3 horizontal;
    vec3 vertical;
    double raioDaLente;
    vec3 u,v,w;
    double tempo0, tempo1; //abertura e fechamento

  public:
    camera(
        point3 olho,
        point3 cena,
        vec3 vup,
        double vfov,
        double aspectRatio,
        double abertura,
        double distanciaFocal,
        double t0,
        double t1
        ){
      auto theta = grauRad(vfov); 
      auto h = tan(theta/2);
      auto frame_alt = 2.0 * h;
      auto frame_comp = aspectRatio * frame_alt;

      auto w = unit_vector(olho - cena);
      auto u = unit_vector(cross(vup, w));
      auto v = cross(w, u);

      origem = olho; 
      horizontal = distanciaFocal * frame_comp * u;
      vertical = distanciaFocal * frame_alt * v;
      cantoInfEsq = origem - horizontal/2 - vertical/2 - distanciaFocal * w; 

      raioDaLente = abertura/2;

      tempo0 = t0;
      tempo1 = t1;
    }

    ray getRay(double s, double t) const{
      vec3 rd = raioDaLente * randomUnitDisk();
      vec3 offset = u * rd.x() + v * rd.y();

      return ray(
          origem + offset, 
          cantoInfEsq + s*horizontal + t*vertical - origem - offset,
          randDouble(tempo0, tempo1)
          );
    }
};
#endif
