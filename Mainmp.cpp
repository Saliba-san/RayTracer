#include "camera.h"
#include "color.h"

#include "util.h"
#include "bvh.h"
#include "cenas.h"

#include <iostream>
#include <stdlib.h>

color rayColor(const ray& r, const color& background, const objeto& mundo, int depth) {
  hitRecord rec;
  if (depth <= 0)
    return color(0,0,0);

  if (!mundo.hit(r, 0.001, infinity, rec))
      return background;

  ray refletido;
  color atenuacao;
  color raioEmitido = rec.ptrMat -> emitido(rec.u, rec.v, rec.p);

  if(!rec.ptrMat->espalhamento(r, rec, atenuacao, refletido))
    return raioEmitido;

  return raioEmitido + atenuacao * rayColor(refletido, background, mundo, depth-1);
}

void write_image(uint8_t *image, int nx, int ny) {
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; ++i) {
            std::cout << static_cast<int>(image[3*(j*nx+i)+0]) << " ";
            std::cout << static_cast<int>(image[3*(j*nx+i)+1]) << " ";
            std::cout << static_cast<int>(image[3*(j*nx+i)+2]) << std::endl;
        }
    }
}

int main() {
  const auto aspectRatio = 16.0/9.0;
  const int comp = 1980;
  const int alt = static_cast<int>(comp/aspectRatio);
  const int samplesPorPixel = 10;
  const int profundidade = 100;
  const int etaMeio = 1.0;

  //auto mundo = random_scene();
  //auto mundo = terra();
  //auto mundo = two_spheres();
  //auto mundo = luzSimples();
  //auto mundo = cornellBox();
  //listaObjetos world;
  //world.add(make_shared<nodeBVH>(mundo, 0, 1));
  auto world = nextWeekFinalScene();

  point3 lookfrom(478,278,-600);
  point3 lookat(278,278,0);
  vec3 vup(0,1,0);
  auto dist_to_focus = 10.0;
  auto aperture = 0.0;
  auto vfov = 35.0;
  const color background(0,0,0); 

  camera cam(lookfrom, lookat, vup, vfov, aspectRatio, aperture, dist_to_focus,0.0,1.0);
  std::vector<uint8_t> image(alt * comp * 3);
  #pragma omp parallel for 
  for (int j = alt-1; j >= 0; j--) {
    for (int i = 0; i < comp; i++) {
      color pixelColor(0,0,0);
      for(int s = 0; s < samplesPorPixel; s++){
        auto u = (i + randDouble()) / (comp); 
        auto v = (j + randDouble()) / (alt);
        ray r = cam.getRay(u, v);
        pixelColor += rayColor(r,background, world, profundidade);
      }
    pixelColor /= double(samplesPorPixel);
    pixelColor = vec3( sqrt(pixelColor[0]), sqrt(pixelColor[1]), sqrt(pixelColor[2]) );

    image[3*(j*comp+i)+0] = int(256 * clamp(pixelColor[0], 0.0, 0.999));
    image[3*(j*comp+i)+1] = int(256 * clamp(pixelColor[1], 0.0, 0.999));
    image[3*(j*comp+i)+2] = int(256 * clamp(pixelColor[2], 0.0, 0.999));

    }	
  }
  write_image(&image[0], comp, alt);
  std::cerr << "\nDone.\n";
}
