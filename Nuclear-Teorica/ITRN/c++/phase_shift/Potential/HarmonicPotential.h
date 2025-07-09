// Archivo: HarmonicPotential.h
#ifndef HARMONIC_POTENTIAL_H
#define HARMONIC_POTENTIAL_H

class HarmonicPotential {
public:
  HarmonicPotential();
  double operator()(double r) const;

private:
  const double w = 1;
};

#endif
