#include <cmath>
#include "Potential.h"
#include "../constants.h"

Potential::Potential() {}

double Potential::nuclear(double r) const {
    return V0 * std::exp(-std::pow(r / a_p, 2));
}

double Potential::coulomb(double r) const {
    return (Z_P * Z_T * e2) / r;
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
