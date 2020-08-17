#ifndef LISTAOBJETOS_H
#define LISTAOBJETOS_H

#include "objeto.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class listaObjetos : public objeto{
  public:
    std::vector<shared_ptr<objeto>> objetos;

  public:
    listaObjetos() {}
    listaObjetos(shared_ptr<objeto> p_objeto) {add(p_objeto);}

    void clear() {objetos.clear();}
    void add(shared_ptr<objeto> p_objeto) {objetos.push_back(p_objeto);}

    virtual bool hit(const ray& r, double tmin, double tmax, hitRecord& rec) const;
    virtual bool boundingBox(double t0, double t1, aabb& outputBox) const;

};
bool listaObjetos::hit(const ray& r, double t_min, double t_max, hitRecord& rec) const{
  hitRecord temp_rec;
  bool hitCheck = false;
  auto maisRecente = t_max;

  for(const auto& v_objeto : objetos){
    if (v_objeto->hit(r, t_min, maisRecente, temp_rec)){
      hitCheck = true;
      maisRecente = temp_rec.t;
      rec = temp_rec;
    }
  }
  return hitCheck;
}

bool listaObjetos::boundingBox(double t0, double t1, aabb& outputBox) const {
  if (objetos.empty()) return false;

  aabb tempBox;
  bool firstBox = true;

  for (const auto& object : objetos) {
    if (!object->boundingBox(t0, t1, tempBox)) return false;
    outputBox = firstBox? tempBox : tempBox.surroundingBox(outputBox, tempBox);
    firstBox = false;
  }
  return true;
}

#endif
