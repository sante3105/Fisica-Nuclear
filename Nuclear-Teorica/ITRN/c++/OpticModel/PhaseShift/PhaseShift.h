#ifndef PHASESHIFT_H
#define PHASESHIFT_H
#include <vector>
#include <complex>

double Sommerfield(double E);
std::complex<double> computePhaseShift(const std::vector<std::complex<double>>& u,
                                       const std::vector<std::complex<double>>& du, 
                                       double E, double a, double l);
#endif
