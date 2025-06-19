#include <cmath>
#include "Potential.h"


Potential::Potential() {}

double Potential::nuclear(double r) const {
    return V0 * std::exp(-std::pow(r / a, 2));
}

double Potential::coulomb(double r) const {
    return Z1Z2e2 / r;
}

double Potential::operator()(double r) const {
    return nuclear(r) + coulomb(r);
}
