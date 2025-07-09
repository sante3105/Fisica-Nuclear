#include <cmath>
#include "Potential.h"
#include <stdexcept> // for std::invalid_argument
#include "../constants.h"

Potential::Potential() {}

double Potential::nuclear(double r) const {
    return V0 * std::exp(-std::pow(r / a_p, 2));
}

double Potential::coulomb(double r) const {
  if (r == 0.0) {
        throw std::invalid_argument("r must be non-zero to avoid division by zero");
    }
  return (Z_P * Z_T * std::erf(r / 1.33)) / r ;// ALPHA + ALPHA 
 // return (Z_P * Z_T * e2) / r;// C12 + p
}


double Potential::angular(double r, int l, double mua) const {
    double constant;
    if (mua == 0.0) {
        constant = (hbar * hbar) / (2.0 * mu); // usa mu fijo de constants.h
    } else {
        constant = 20.736 / mua;
    }

    if (l == 0) return 0.0;
    return constant * l * (l + 1) / (r * r);
}

double Potential::operator()(double r, int l, double mua) const {
    return nuclear(r) + coulomb(r) + angular(r, l, mua);
}
