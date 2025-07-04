#include <cmath>
#include "HarmonicPotential.h"

HarmonicPotential::HarmonicPotential() {}

double  HarmonicPotential::operator()(double r) const {
  return r * r * w;
    }
