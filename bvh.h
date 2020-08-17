#ifndef BVH_H
#define BVH_H

#include "listaObjetos.h"
#include <algorithm>

class nodeBVH : public objeto {
  public:
    shared_ptr<objeto> left;
    shared_ptr<objeto> right;
    aabb box;

  public:
    nodeBVH();

    nodeBVH(listaObjetos& lista, double t0, double t1):
      nodeBVH(lista.objetos, 0, lista.objetos.size(), t0, t1) {}

    nodeBVH(
        std::vector<shared_ptr<objeto>>& objetos,
        size_t start, size_t end, double time0, double time1
        );

    virtual bool hit(const ray& r, double tmin, double tmax, hitRecord& rec) const;
    virtual bool boundingBox(double t0, double t1, aabb& outputBox) const;
};

bool nodeBVH::hit(const ray& r, double tmin, double tmax, hitRecord& rec) const{
  if(!box.hit(r,tmin, tmax))
    return false;
  bool hitLeft = left->hit(r, tmin, tmax, rec);
  bool hitRight = right -> hit(r, tmin, hitLeft? rec.t : tmax, rec);
  return hitLeft || hitRight;
}

bool nodeBVH::boundingBox(double t0, double t1, aabb& outputBox) const{
  outputBox = box;
  return true;
}

inline bool box_compare(const shared_ptr<objeto> a, const shared_ptr<objeto> b, int axis) {
    aabb box_a;
    aabb box_b;

    if (!a->boundingBox(0,0, box_a) || !b->boundingBox(0,0, box_b))
        std::cerr << "No bounding box in nodeBVH constructor.\n";

    return box_a.min().e[axis] < box_b.min().e[axis];
}

bool box_x_compare (const shared_ptr<objeto> a, const shared_ptr<objeto> b) {
    return box_compare(a, b, 0);
}

bool box_y_compare (const shared_ptr<objeto> a, const shared_ptr<objeto> b) {
    return box_compare(a, b, 1);
}

bool box_z_compare (const shared_ptr<objeto> a, const shared_ptr<objeto> b) {
    return box_compare(a, b, 2);
}

nodeBVH::nodeBVH(
    std::vector<shared_ptr<objeto>>& objetos,
    size_t start, size_t end, double time0, double time1
    ) {
  int axis = std::rand() % 3;
  auto comparador = (axis == 0)? box_x_compare : (axis == 1)? box_y_compare : box_z_compare;

  size_t object_span = end - start;

  if (object_span == 1) {
    left = right = objetos[start];
  }else if (object_span == 2){
    if (comparador(objetos[start], objetos[start+1])) {
      left = objetos[start];
      right = objetos[start+1];
    }else {
      left = objetos[start+1];
      right = objetos[start];
    }
  } else {
    std::sort(objetos.begin() + start, objetos.begin() + end, comparador);

    auto mid = start + object_span/2;
    left = make_shared<nodeBVH>(objetos, start, mid, time0, time1);
    right = make_shared<nodeBVH>(objetos, mid, end, time0, time1);
  }
  aabb box_left, box_right;

  if(!left->boundingBox(time0,time1, box_left)|| !right->boundingBox(time0, time1, box_right))
    std::cerr << "No boundingBox in nodeBVH constructor. \n";

  box = box.surroundingBox(box_left, box_right);
};

#endif 
