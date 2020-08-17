#ifndef TEXTURE_H
#define TEXTURE_H

#include "util.h"
#include "vec3.h"

#include "perlin.h"
#include "external/rtw_stb_image.h"

class texture {
  public:
    virtual color value (double u, double v, const point3& p) const = 0;
};

class solidColor : public texture {
  private:
    color colorValue;

  public:
    solidColor() {}
    solidColor(color c) : colorValue(c) {}

    solidColor(double r, double g, double b) : solidColor(color(r,g,b)) {}

    virtual color value(double u, double v, const vec3& p) const{
      return colorValue;
    }
};

class quadriculado : public texture {
  private:
    shared_ptr<texture> even;
    shared_ptr<texture> odd;

  public:
    quadriculado() {}
    quadriculado(shared_ptr<texture> t0, shared_ptr<texture> t1): even(t0), odd(t1) {}

    virtual color value(double u, double v, const point3& p) const {
      auto sines = sin(10*p.x()) * sin(10*p.y()) * sin(10*p.z());
      if (sines < 0)
        return odd -> value(u,v,p);
      else
        return even -> value(u,v,p);
    }
};

class noise : public texture {
  public:
    perlin perlinNoise;
    double scale;

  public:
    noise() {}
    noise(double sc) : scale(sc) {}
    virtual color value(double u, double v, const point3& p) const {
      //marmore
      return (color(1,1,1) * 0.5 * (1 + sin(scale*p.z() + 10*perlinNoise.turb(p))));
    }
};

class imagem_texture : public texture {
  private:
    unsigned char* data;
    int comp, alt;
    int bytesPorScanline;

  public:
    const static int bytesPorPixel = 3;

    imagem_texture() :
      data(nullptr), comp(0), alt(0), bytesPorScanline(0) {};

    imagem_texture(const char* filename) {
      auto componentesPorPixel = bytesPorPixel;

      data = stbi_load(
          filename, &comp, &alt, &componentesPorPixel, componentesPorPixel);

      if(!data) {
        std::cerr << "Erro: imagem nao carregou '" << filename << "'.\n";
        comp = alt = 0;
      }
      bytesPorScanline = bytesPorPixel * comp;
    }

    ~imagem_texture() {
      delete data;
    }

    virtual color value(double u, double v, const vec3& p) const {
      if (data == nullptr) return color(0,1,1);

      u = clamp(u, 0.0, 1.0);
      v = 1.0 - clamp(v, 0.0, 1.0);

      auto i = static_cast<int>(u*comp);
      auto j = static_cast<int>(v*alt);

      if (i >= comp) i = comp-1;
      if (j >= alt) j = alt-1;

      const auto colorScale = 1.0/255.0;
      auto pixel = data + j*bytesPorScanline + i*bytesPorPixel;

      return color(colorScale*pixel[0], colorScale*pixel[1], colorScale*pixel[2]);
    }
};

#endif
