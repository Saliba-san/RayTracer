#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"
#include "objeto.h"
#include "texture.h"

class material {
  public:
    virtual bool espalhamento(const ray& r_in, const hitRecord& rec, color& atenuacao, ray& espalhamento)
      const = 0;

    virtual color emitido(double u, double v, const point3& p) const {
      return color(0,0,0);
    }
};

class lambertian : public material {
  public:
    shared_ptr<texture> albedo;
    
  public:
    lambertian(shared_ptr<texture> a) : albedo(a) {}
    
    virtual bool espalhamento(const ray& r_in, const hitRecord& rec, color& atenuacao, ray& espalhamento)
      const {
        vec3 dirEspalhamento = rec.normal + randomUnitVector();
        espalhamento = ray(rec.p, dirEspalhamento, r_in.tempo());
        atenuacao = albedo -> value(rec.u, rec.v, rec.p);
        return true;
      }
};

class metal : public material{
  public:
    color albedo;
    double fuzz;
  public:
    metal(const color& a, double f): albedo(a), fuzz(f<1? f : 1)  {}

    virtual bool espalhamento(const ray& r_in, const hitRecord& rec, color& atenuacao, ray& espalhamento)
      const {
        vec3 refletido = reflexao(unit_vector(r_in.direcao()), rec.normal); 
        espalhamento = ray(rec.p, refletido + fuzz*randomUnitSphere(), r_in.tempo());
        atenuacao = albedo;
        return (dot(espalhamento.direcao(), rec.normal) > 0);
      }
};

double schlick(double cos, double razaoEtas){
  auto r0 = (1-razaoEtas)/(1+razaoEtas); 
  r0 = r0*r0;
  return r0 + (1-r0)*pow((1-cos), 5);
}

class dieletric : public material{
  public:
    double etaIn;
    double etaOut;
  public:
    dieletric(double etai, double etao) : etaIn(etai), etaOut(etao) {}

    virtual bool espalhamento(const ray& r_in, const hitRecord& rec, color& atenuacao, ray& espalhamento)
      const {
        atenuacao = color(1.0,1.0,1.0);
        double razaoEtas;
        if(rec.frente) {
          razaoEtas = etaOut / etaIn;
        } else{
          razaoEtas = etaIn / etaOut;
        }
	
        vec3 unit_dir = unit_vector(r_in.direcao());
        double cosTheta = fmin(dot(-unit_dir, rec.normal), 1.0);
        double senTheta = sqrt(1.0 - cosTheta * cosTheta);
        if(razaoEtas * senTheta > 1.0) {
          vec3 refletido = reflexao(unit_dir, rec.normal);
          espalhamento = ray(rec.p, refletido, r_in.tempo());
          return true;
        }

        double reflect_prob = schlick(cosTheta, razaoEtas);
        if (randDouble() < reflect_prob){
          vec3 reflected = reflexao(unit_dir, rec.normal);
          espalhamento = ray(rec.p, reflected, r_in.tempo());
          return true;
        }

        vec3 refratado = refracao(unit_dir, rec.normal, razaoEtas);
        espalhamento = ray(rec.p, refratado, r_in.tempo());
        return true;
      }
};

class fonteDifusa : public material {
  public:
    shared_ptr<texture> emit;

  public:
    fonteDifusa(shared_ptr<texture> a) : emit(a) {}

    virtual bool espalhamento(
        const ray& r_in, const hitRecord& rec, color& atenuacao, ray& espalhado) const {
      return false;
    }

    virtual color emitido(double u, double v, const point3& p) const {
      return emit -> value(u,v,p);
    }
};

class isotropic : public material {
  public:
    shared_ptr<texture> albedo;

  public:
    isotropic(shared_ptr<texture> a) : albedo (a) {}

    virtual bool espalhamento (
        const ray& r_in, const hitRecord& rec, color& atenuacao, ray& espalhado) const {
      espalhado = ray(rec.p, randomUnitSphere(), r_in.tempo());
      atenuacao = albedo -> value(rec.u, rec.v, rec.p);
      return true;
    }
};


#endif
