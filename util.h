#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constantes

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.141592653589793238462643383279502884197169399375105820974944;

//Funcoes uteis
inline double grauRad(double grau){
  return grau*pi/180;
}

inline double randDouble(){
  return rand() / (RAND_MAX + 1.0);
}

inline double randDouble(double min, double max){
  return min + (max-min)*randDouble();
}

inline double clamp(double x, double min, double max){
  if(x<min) return min;
  if(x>max) return max;
  return x;
}
// Headers comuns

#endif
