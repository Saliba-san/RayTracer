#ifndef COLOR_H
#define COLOR_H

#include "util.h"
#include "vec3.h"

#include <iostream>

void writeColor(std::ostream &out, color rgb, int samplesPorPixel){
  auto r = rgb.x();
  auto g = rgb.y();
  auto b = rgb.z(); 

  auto escala = 1.0/samplesPorPixel;
  r = sqrt(r*escala);
  g = sqrt(g*escala);
  b = sqrt(b*escala);

  out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
    << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
    << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif

